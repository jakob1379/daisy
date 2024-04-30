// litter_residue_component.C -- Dynamic litter layer affecting water and heat.
// 
// Copyright 2010 KU
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

#include "litter.h"
#include "block_model.h"
#include "mathlib.h"
#include "librarian.h"
#include "check.h"
#include "log.h"
#include "organic.h"
#include "models/litter/litter_residue_model.h"

struct LitterResidueComponent : Litter, LitterResidueModel
{
  void output (Log& log) const
  {
    LitterResidueModel::output (log);
  }

  LitterResidueComponent (const BlockModel& al)
    : Litter(al),
      LitterResidueModel(al.number ("water_capacity"),
                         al.number ("vapor_flux_factor"),
                         al.number ("specific_AI"),
                         al.number ("extinction_coefficent"),
                         al.number ("albedo", -1.0))
  { }  
  ~LitterResidueComponent ()
  { }
};

static struct LitterResidueSyntax : DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new LitterResidueComponent (al); }
  LitterResidueSyntax ()
    : DeclareModel (Litter::component, "residue", "\
A dynamic litter layer based on applied fertilizer and crop residuals.\n\
\n\
A 'mulch area index' is calculated from the surface organic\n\
matericals, and from that a mulch cover is calculated based on Beer's law\n\
similarily to how the crop cover is calculated from the leaf area\n\
index.")
  { }
  void load_frame (Frame& frame) const
  { 
    frame.set_strings ("cite", "scopel2004");

    frame.declare ("water_capacity", "L/kg", Check::non_negative (),
		   Attribute::Const, "\
Water holding capacity of surface residulas.");
    frame.declare_fraction ("vapor_flux_factor", Attribute::Const, "\
Reduction factor for potential evaporation below litter.\n\
Only area covered by residue is affected.");
    frame.set ("vapor_flux_factor", 0.0);
    frame.declare ("specific_AI", "m^2/kg DM", Check::non_negative (),
		   Attribute::Const, "\
Area covered per litter mass.");
    frame.declare ("extinction_coefficent", Attribute::None (),
		   Check::positive (), Attribute::Const, "\
Beer's law extinction coefficient for litter.");
    frame.declare ("albedo", Attribute::None (), Check::positive (),
                   Attribute::OptionalConst, "Reflection factor.\n\
By default, the surface albedo will be used.");

    frame.declare ("mass", "kg DM/m^2", Attribute::LogOnly, "\
Total mass of mulch layer.");
  }
} LitterResidue_syntax;

// The 'Millet' parameterization.
  
static struct LitterMilletsyntax : public DeclareParam
{ 
  LitterMilletsyntax ()
    : DeclareParam (Litter::component, "Millet", "residue", "\
Millet crop residues in Planaltina.")
  { }
  void load_frame (Frame& frame) const
  {
    frame.set_strings ("cite", "macena2003");

    frame.set ("water_capacity", 3.2);
    frame.set ("specific_AI", 3.9);
    frame.set ("extinction_coefficent", 0.45);
  }
} LitterMillet_syntax;

// The 'Maize' parameterization.

static struct LitterMaizesyntax : public DeclareParam
{ 
  LitterMaizesyntax ()
    : DeclareParam (Litter::component, "Maize", "residue", "\
Maize crop residues in La Tinaja.")
  { }
  void load_frame (Frame& frame) const
  {
    frame.set_strings ("cite", "scopel1998");

    frame.set ("water_capacity", 3.8);
    frame.set ("specific_AI", 3.7);
    frame.set ("extinction_coefficent", 0.80);
  }
} LitterMaize_syntax;
