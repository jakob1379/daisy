// reaction_Python.C -- Python interface for chemical reactions.
// 
// Copyright 2025 Per Abrahamsen and KU.
//
// This file is part of Daisy.
// 
// Daisy is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
// 
// Daisy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser Public License for more details.
// 
// You should have received a copy of the GNU Lesser Public License
// along with Daisy; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#define BUILD_DLL

#include "daisy/chemicals/reaction.h"
#include "object_model/block_model.h"
#include "daisy/chemicals/chemistry.h"
#include "daisy/chemicals/chemical.h"
#include "daisy/soil/soil.h"
#include "daisy/soil/soil_water.h"
#include "daisy/output/log.h"
#include "util/assertion.h"
#include "object_model/librarian.h"
#include "object_model/treelog.h"
#include "object_model/frame.h"
#include "object_model/frame_submodel.h"
#include "object_model/vcheck.h"
#include "daisy/soil/soil_heat.h"
#include "daisy/soil/transport/geometry.h"
#include "daisy/organic_matter/organic.h"
#include <pybind11/embed.h>
#include <pybind11/stl.h>


struct ReactionPython : public Reaction
{
  const symbol pmodule;
  const symbol psoil;

  
  const std::vector<symbol> solute_in_name;
  typedef decltype(&Chemical::C_primary) in_handle_t;
  const std::vector<in_handle_t> solute_in_handle;
  const std::vector<symbol> solute_in_tag;
  
  const std::set<symbol> extra;
  
  const std::vector<symbol> solute_out_name;
  typedef decltype(&Chemical::add_to_transform_source) out_handle_t;
  const std::vector<out_handle_t> solute_out_handle;
  const std::vector<symbol> solute_out_tag;
  
  const symbol ptop;

  // State
  pybind11::object py_module;
  pybind11::object py_soil;
  pybind11::object py_top;
  enum class state_t { uninitialized, working, error } state;

  // Output.
  void output (Log& log) const
  { }

  // Simulation.
  void tick_soil (const Geometry& geo,
                  const Soil& soil, const SoilWater& soil_water, 
                  const SoilHeat& soil_heat, OrganicMatter& organic_matter,
                  Chemistry& chemistry, const double dt, Treelog& msg)
  {
    if (psoil == Attribute::None () || state != state_t::working)
      return;

#if 0
    Chemical& soil_NO3 = chemistry.find (Chemical::NO3 ());
#endif

    TREELOG_MODEL (msg);
    const size_t cell_size = soil.size ();
    pybind11::dict kwargs;
    for (size_t i = 0; i < cell_size; i++)
      {

	if (extra.find ("Theta") != extra.end ())
	  kwargs["Theta"] = soil_water.Theta (i);
	if (extra.find ("h") != extra.end ())
	  kwargs["h"] = soil_water.h (i);
	if (extra.find ("rho_b") != extra.end ())
	  kwargs["rho_b"] = soil.dry_bulk_density (i);
	if (extra.find ("z") != extra.end ())
	  kwargs["z"] = geo.cell_z (i);
	if (extra.find ("CO2_C") != extra.end ())
	  kwargs["CO2_C"] = organic_matter.CO2 (i);
	if (extra.find ("CO2_C_fast") != extra.end ())
	  kwargs["CO2_C_fast"] = organic_matter.CO2_fast (i);
	if (extra.find ("T") != extra.end ())
	  kwargs["T"] = soil_heat.T (i);
	if (extra.find ("SMB_C") != extra.end ())
	  kwargs["SMB_C"] = organic_matter.get_smb_c_at (i);

	try
	  {
	    pybind11::object py_object = py_soil (**kwargs);
	  }
	catch (...)
	  {
	    msg.error ("Call to Python function '"
		       + psoil + "' in '" + pmodule + "' failed");
	    state = state_t::error;
	  }
      }
  }

  void tick_top (const Vegetation& vegetation,
		 const Bioclimate& bioclimate,
		 const double tillage_age /* [d] */,
                 const double total_rain, 
                 const double h_pond,
                 OrganicMatter&,
		 Chemistry& chemistry, const double dt, Treelog& msg)
  {
    if (ptop == Attribute::None () || state != state_t::working)
      return;

    try
      {
	pybind11::object py_object = py_top ();
      }
    catch (...)
      {
	msg.error ("Call to Python function '"
		   + ptop + "' in '" + pmodule + "' failed");
	state = state_t::error;
      }
    return;
  }

  // Create.
  bool check (const Geometry& geo, 
              const Soil& soil, const SoilWater& soil_water,
	      const SoilHeat& soil_heat,
	      const OrganicMatter&, const Chemistry& chemistry,
	      Treelog& msg) const
  { 
    TREELOG_MODEL (msg);
    bool ok = true;
    for (symbol name : solute_in_name)
      {
	if (!chemistry.know (name))
	  {
	    msg.error ("Unknown input chemical '" + name + "'");
	    ok = false;
	  }
      }
    for (symbol name : solute_out_name)
      {
	if (!chemistry.know (name))
	  {
	    msg.error ("Unknown output chemical '" + name + "'");
	    ok = false;
	  }
      }
    return ok;
  }
  void initialize (const Geometry& geo, 
                   const Soil& soil, const SoilWater& soil_water,
                   const SoilHeat& soil_heat, const OrganicMatter&,
		   const Chemistry& chemistry,
		   const Surface&, Treelog& msg)
  {
    switch (state)
      {
      case state_t::error:
      case state_t::working:
	return;
      case state_t::uninitialized:
	// Find module.
	try
	  {
	    py_module = pybind11::module::import (pmodule.name ().c_str ());
	  }
	catch (...)
	  {
	    Assertion::message ("Could not find Python module '"
				+ pmodule + ".");
	    state = state_t::error;
	    return;
	  }

	// Find soil
	if (psoil != Attribute::None ())
	  try
	    {
	      py_soil = py_module.attr(psoil.name ().c_str ());
	    }
	  catch (...)
	    {
	      Assertion::message ("Can't find Python function '"
				  + psoil + "' in '" + pmodule + "'.");
	      state = state_t::error;
	      return;
	    }
	
	// Find top
	if (ptop != Attribute::None ())
	  try
	    {
	      py_top = py_module.attr(ptop.name ().c_str ());
	    }
	  catch (...)
	    {
	      Assertion::message ("Can't find Python function '"
				  + ptop + "' in '" + pmodule + "'.");
	      state = state_t::error;
	      return;
	    }
      }
    state = state_t::working;
  }
  static std::set<symbol> v2s (const std::vector<symbol>& v)
  { return std::set (v.begin (), v.end ()); }
  static std::vector<symbol> extract_chemical (const BlockModel& al,
					       const symbol key)
  {
    const auto seq = al.submodel_sequence (key);
    std::vector<symbol> result;
    for (auto i: seq)
      result.push_back (i->name ("chemical"));
    return result;
  }
  static std::vector<in_handle_t> extract_in_handle (const BlockModel& al)
  {
    const auto seq = al.submodel_sequence ("solute_in");
    static std::vector<in_handle_t> result;
    for (auto i: seq)
      {
	if (!i->check ("handle"))
	  {
	    result.push_back (&Chemical::C_average);
	    continue;
	  }
	const symbol handle = i->name ("handle");
	if (handle == "C_primary")
	  result.push_back (&Chemical::C_primary);
	else if (handle == "C_secondary")
	  result.push_back (&Chemical::C_secondary);
	else if (handle == "C_average")
	  result.push_back (&Chemical::C_average);
	else if (handle == "M_primary")
	  result.push_back (&Chemical::M_primary);
	else if (handle == "M_secondary")
	  result.push_back (&Chemical::M_secondary);
	else if (handle == "M_total")
	  result.push_back (&Chemical::M_total);
	else
	  {
	    result.push_back (&Chemical::C_average);
	    daisy_bug ("Unknown in handle '" + handle + "'");
	  }
      }
    return result;
  }
  static std::vector<symbol> extract_tag (const BlockModel& al,
					  const symbol key)
  {
    const auto seq = al.submodel_sequence (key);
    std::vector<symbol> result;
    for (auto i: seq)
      result.push_back (i->name ("tag", i->name ("chemical")));
    return result;
  }

  static std::vector<out_handle_t> extract_out_handle (const BlockModel& al)
  {
    const auto seq = al.submodel_sequence ("solute_out");
    static std::vector<out_handle_t> result;
    for (auto i: seq)
      {
	if (!i->check ("handle"))
	  {
	    result.push_back (&Chemical::add_to_transform_source);
	    continue;
	  }
	const symbol handle = i->name ("handle");
	if (handle == "primary")
	  result.push_back (&Chemical::add_to_transform_source);
	else if (handle == "secondary")
	  result.push_back (&Chemical::add_to_transform_source_secondary);
	else 
	  {
	    result.push_back (&Chemical::add_to_transform_source);
	    daisy_bug ("Unknown out handle '" + handle + "'");
	  }
      }
    return result;
  }

  explicit ReactionPython (const BlockModel& al)
    : Reaction (al),
      pmodule (al.name ("module")),
      psoil (al.name ("soil", Attribute::None ())),
      solute_in_name (extract_chemical (al, "solute_in")),
      solute_in_handle (extract_in_handle (al)),
      solute_in_tag (extract_tag (al, "solute_in")),
      extra (v2s (al.name_sequence ("extra"))),
      solute_out_name (extract_chemical (al, "solute_out")),
      solute_out_handle (extract_out_handle (al)),
      solute_out_tag (extract_tag (al, "solute_out")),
      ptop (al.name ("top", Attribute::None ())),
      state (state_t::uninitialized)
  { }
};

static struct ReactionPythonSyntax : public DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new ReactionPython (al); }
  ReactionPythonSyntax ()
    : DeclareModel (Reaction::component, "Python", "\
Python interface for chemical reactions above and below ground.")
  { }
  static void load_solute_in (Frame& frame)
  {
    frame.declare_string ("chemical", Attribute::Const, "\
Name of chemical.");
    static const VCheck::InLibrary is_chemical (Chemical::component);
    frame.set_check ("chemical", is_chemical);
    frame.declare_string ("handle", Attribute::OptionalConst, "\
How to access it.\n\
One of 'C_primary', 'C_secondary', 'C_average',\n\
'M_primary', 'M_secondary', or 'M_total'.");
    static const VCheck::Enum is_handle
      ("C_primary", "C_secondary", "C_average",
       "M_primary", "M_secondary", "M_total");
    frame.set_check ("handle", is_handle);
    frame.declare_string ("tag", Attribute::OptionalConst, "\
Nmae to use as a Python parameter.");
    frame.order ("chemical", "handle", "tag");
  }

  static void load_texture (Frame& frame)
  {
    frame.declare_string ("tag", Attribute::Const, "\
Name of Python parameter.");
    frame.declare ("size", "um", Attribute::Const, "\
Include particles below this size.");
    frame.order ("tag", "size");
  }

  static void load_solute_out (Frame& frame)
  {
    frame.declare_string ("chemical", Attribute::Const, "\
Name of chemical.");
    static const VCheck::InLibrary is_chemical (Chemical::component);
    frame.set_check ("chemical", is_chemical);
    frame.declare_string ("handle", Attribute::OptionalConst, "\
Where to add it.\n\
One of 'primary', 'secondary'.");
    static const VCheck::Enum is_handle ("primary", "secondary");
    frame.set_check ("handle", is_handle);
    frame.declare_string ("tag", Attribute::OptionalConst, "\
Name to use both for logging and as Python output.");
    frame.order ("chemical", "handle", "tag");
  }

  static void load_gas_out (Frame& frame)
  {
    frame.declare_string ("tag", Attribute::Const, "\
Name to use both for logging and as Python output.");
    frame.order ("tag");
  }

  void load_frame (Frame& frame) const
  {
    frame.declare_string ("module", Attribute::Const, "\
Python module to find the functions.");
    frame.declare_string ("soil", Attribute::OptionalConst, "\
Name of Python function for soil reactions.");
    frame.declare_submodule_sequence ("solute_in", Attribute::Const, "\
List of solutes to pass to Python as input.", load_solute_in);
    frame.declare_submodule_sequence ("texture", Attribute::Const, "\
List of texture classes to pass to Python.", load_texture);
    frame.declare_string ("extra",
			  Attribute::Const, Attribute::Variable, "\
Extra input parameters to Python soil function.\n\
\n\
Options include:\n\
\n\
  z [cm]: Height above soil surface (always negative).\n\
  rho_b [g S/cm^3]: Soil dry bulk density.\n\
  Theta [cm^3 W/cm^3 V]: Water per system volume.\n\
  h [cm]: Soil water potential.\n\
  T [dg C]: Temperature.\n\
  CO2_C [g C/cm^3 V/h]: CO2 production rate per system volume.\n\
  CO2_C_fast [g C/cm^3 V/]: Fast CO2 production rate per system volume.\n\
  SMB_C [g C/cm^3 V]: Fast pool SMB carbon per system volume.");
    static struct ExtraCheck : public VCheck::Enum
    {
      ExtraCheck ()
	: VCheck::Enum ()
      {
	add ("z");
	add ("rho_b");
	add ("Theta");
	add ("h");
	add ("T");
	add ("CO2_C");
	add ("CO2_C_fast");
	add ("SMB_C");
      }
    } extra_check;
    frame.set_check ("extra", extra_check);
    frame.declare_submodule_sequence ("solute_out", Attribute::Const, "\
List of solute sources generated by Python as output.", load_solute_out);
    frame.declare_submodule_sequence ("gas_out", Attribute::Const, "\
List of gasses produced.", load_gas_out);
    frame.declare_string ("top", Attribute::OptionalConst, "\
Name of Python function for above ground reactions.");
  }
} ReactionPython_syntax;
