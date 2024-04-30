#include <sstream>
#include "abiotic.h"
#include "block_model.h"
#include "check.h"
#include "chemical.h"
#include "librarian.h"
#include "log.h"
#include "mathlib.h"
#include "litter.h"
#include "models/litter/litter_mulch_model.h"

struct LitterMulchComponent : Litter, LitterMulchModel
{
  void output (Log& log) const
  {
    Litter::output (log);
    output_variable (mass, log);
    output_variable (height, log);
    output_variable (contact, log);
    output_variable (water, log);
    output_variable (protected_water, log);
    output_variable (Theta, log);
    output_variable (h, log);
    output_variable (h1, log);
    output_variable (h_soil, log);
    output_variable (K_soil, log);
    output_variable (E_darcy, log);
    output_variable (h_factor, log);
    output_variable (T, log);
    output_variable (T_soil, log);
    output_variable (T_factor, log);
    output_variable (SMB_C, log);
    output_variable (SMB_factor, log);
    output_variable (factor, log);
    output_variable (DOC_gen, log);
    output_variable (DON_gen, log);
    output_variable (SOC_gen, log);
    output_variable (SON_gen, log);
  }

  static double find_Theta_sat (const Frame& al)
  {
    const double bulk_density = al.number ("density"); // [kg/m^3]
    const double density = al.number ("particle_density", bulk_density); // [kg/m^3]
    const double water_capacity = al.number ("water_capacity"); // [L/kg]
    const double L_per_m3 = 1000.0; // [L/m^3]
    // [m^3/m^3] = [kg/m^3] * [L/kg] / [L/m^3]
    const double Theta_sat = density * water_capacity / L_per_m3; 

    return Theta_sat;
  }


  LitterMulchComponent (const BlockModel& al)
    : Litter (al),
      LitterMulchModel(al.number ("water_capacity"),
                       al.number ("vapor_flux_factor"),
                       al.number ("specific_AI"),
                       al.number ("extinction_coefficent"),
                       al.number ("albedo", -1.0),
                       al.name ("DOC_name"),
                       al.name ("DON_name"),
                       al.number ("density"),
                       al.number ("particle_density", density),
                       al.number ("decompose_height"),
                       al.number ("evaporate_depth"),
                       al.number ("soil_height"),
                       al.number ("Theta_res"),
                       LitterMulchComponent::find_Theta_sat (al.frame ()),
                       al.number ("h_min"),
                       al.number ("factor_exch"),
                       al.number ("alpha"),
                       al.number ("Si"),
                       Librarian::build_item<Retention> (al, "retention"),
                       al.plf ("decompose_heat_factor"),
                       Abiotic::find_T_scale (al), 
                       al.plf ("decompose_water_factor"),
                       al.integer ("decompose_SMB_pool"),
                       al.number ("decompose_SMB_KM"),
                       Abiotic::find_SMB_scale (al),
                       al.flag ("use_soil_decompose"),
                       al.msg ())
  { }
  ~LitterMulchComponent ()
  { }

};
                              
static struct LitterMulchSyntax : DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new LitterMulchComponent (al); }
  LitterMulchSyntax ()
    : DeclareModel (Litter::component, "mulch", "residue", "\
A decomposing mulch layer.\n\
\n\
The bottom of the mulch layer may decompose based on the conditions\n\
in the top of soil.")
  { }
  static bool check_alist (const Metalib& metalib, const Frame& al, Treelog& msg)
  {
    bool ok = true;

    // T_ref
    const double T_ref = al.number ("T_ref");
    const PLF& decompose_heat_factor = al.plf ("decompose_heat_factor");
    const double ref_value = (decompose_heat_factor.size () < 1)
      ? Abiotic::f_T0 (T_ref)
      : decompose_heat_factor (T_ref);

    if (!(ref_value > 0.0))
      {
	std::ostringstream tmp;
	tmp << "heat_factor at " << T_ref << " dg C (T_ref) is " << ref_value
	    << ", should be > 0";
	msg.error (tmp.str ());
	ok = false;
      }

    // SMB_ref
    if (al.check ("SMB_ref"))
      {
	const double SMB_ref = al.number ("SMB_ref");
	const double decompose_SMB_KM = al.number ("decompose_SMB_KM");
	const double ref_value = (decompose_SMB_KM > 0.0)
	  ? SMB_ref / (decompose_SMB_KM + SMB_ref)
	  : 1.0;

	if (!(ref_value > 0.0))
	  {
	    std::ostringstream tmp;
	    tmp << "SMB_factor at " << SMB_ref << " g C/cm^3 is " << ref_value
		<< ", should be > 0";
	    msg.error (tmp.str ());
	    ok = false;
	  }
      }

    // Theta_res / Theta_sat
    const double Theta_res = al.number ("Theta_res");
    const double Theta_sat = LitterMulchComponent::find_Theta_sat (al);
    const double Si = al.number ("Si");
    if (!(Theta_res < Theta_sat))
      {
	std::ostringstream tmp;
	tmp << "Theta_res (" << Theta_res << ") should be < Theta_sat (" << Theta_sat
	    << ")";
	msg.error (tmp.str ());
	ok = false;
      }
    if (Theta_res > Si * Theta_sat)
      {
	msg.error ("Theta_res > Theta_sat * Si");
	ok = false;
      }
    if (Si > 1.0)
      {
	msg.error ("Si > 100 %");
	ok = false;
      }
    return ok;
  }
    
  void load_frame (Frame& frame) const
  { 
    frame.add_check (check_alist);
    frame.set_strings ("cite", "findeling2007modelling");
    frame.declare_string ("DOC_name", Attribute::Const, "\
Name of compound representing dissolved organic carbon.");
    frame.set ("DOC_name", Chemical::DOC ());
    frame.declare_string ("DON_name", Attribute::Const, "\
Name of compound representing dissolved organic nitrogen.");
    frame.set ("DON_name", Chemical::DON ());
    frame.declare ("density", "kg DM/m^3", Check::positive (),
		   Attribute::Const, "\
Bulk density of mulch layer.");
    frame.declare ("particle_density", "kg DM/m^3", Check::positive (),
		   Attribute::OptionalConst, "\
Particle density of mulch layer.\n\
If unspecified, use bulk density instead.");
    frame.declare ("decompose_height", "cm", Check::positive (),
		   Attribute::Const, "\
Height of muclh layer considered in contact with the soil.\n\
Only this part of the mulch layer will decompose");
    frame.declare ("evaporate_depth", "cm", Check::non_negative (),
		   Attribute::Const, "\
Depth into mulch layer affected by evaporation.\n\
Only water in this part of the mulch layer will evaporate.\n\
\n\
The mulch layer is asumed to be filled up from the bottom.\n\
\n\
If W is the amount of water in the mulch, R is the residual \n\
water (Theta_res * C), C is the water capacity, D is this parameter,\n\
and H is the height of the mulch layer, the maximum evaporation (Ea_max)\n\
can be found as:\n\
\n\
Ea_max = max (0, (W - R) - (C - R) * (H - min (D, H)) / H)");
    frame.set ("evaporate_depth", 1000.0);
    frame.declare ("soil_height", "cm", Check::negative (),
		   Attribute::Const, "\
Height of soil layer (a negative number) contributing to decay.");
    frame.declare ("Theta_res", Attribute::None (), Check::non_negative (),
		   Attribute::Const, "\
Water content where biological activity stops");
    frame.declare ("h_min", "cm", Check::negative (),
		   Attribute::Const, "\
Water pressure where biological activity stops.");
    frame.set ("h_min", pF2h (6.5));
    frame.declare ("factor_exch", Attribute::None (), Check::non_negative (),
		   Attribute::Const, "\
Limiting factor when calculating Darcy exchange between mulch and soil.\n\
It is intended to emulate poor contact between the two media.");
    frame.declare ("alpha", Attribute::None (), Check::non_negative (),
		   Attribute::Const, "Interception parameter.\n\
The fraction of water hitting the litter will be determined by:\n\
\n\
  exp (-alpha (Theta_sat - Theta_res) / (Theta_sat - Theta))\n\
\n\
If alpha is 0 (default), all water hitting the canopy will be intercepted.");
    frame.set ("alpha", 0.0);

    frame.declare_fraction ("Si", Attribute::Const, "\
Water content where diffusion to wash off begins relative to Theta_sat.\n\
Theta_sat = 100 %");
    frame.declare_object ("retention", Retention::component,
                          "The retention curve to use.");
    frame.set ("retention", "PASTIS");
    Abiotic::load_frame (frame);
    frame.declare_boolean ("use_soil_decompose", Attribute::Const, "\
Use temperature and moisture of top soil for turnover and decomposition.\n\
The depth of the top soil is determined by 'soil_height'.");
    frame.set ("use_soil_decompose", true);
    
    // Log variables.
    frame.declare ("height", "cm", Attribute::LogOnly, "\
Total height of mulch layer.");
    frame.declare_fraction ("contact", Attribute::LogOnly, "\
Fraction of mulch layer in contact with soil.\n\
DOM in this part of the mulch is degraded.");
    frame.declare ("water", "mm", Attribute::LogOnly, "\
Total water in mulch.");
    frame.declare ("protected_water", "mm", Attribute::LogOnly, "\
Amount of water potentially protected from evaporation.\n\
Only when the water content exceeds this amount it will evaporate.");
    frame.declare ("Theta", Attribute::None (), Attribute::LogOnly, "\
Relative water content.");
    frame.declare ("h", "cm", Attribute::LogOnly, "\
Mulch water potential at stat of timestep.");
    frame.declare ("h1", "cm", Attribute::LogOnly, "\
Estimated mulch water potential at end of timestep.");
    frame.declare ("h_soil", "cm", Attribute::LogOnly, "\
Soil water potential.");
    frame.declare ("K_soil", "cm/h", Attribute::LogOnly, "\
Soil water conductivity.");
    frame.declare ("E_darcy", "cm/h", Attribute::LogOnly, "\
Potential mulch-soil water exchange as calculated by modified Darcy.\n\
Positive down.");
    frame.declare ("h_factor", Attribute::None (), Attribute::LogOnly, "\
Water potential effect on turnover.");
    frame.declare ("T", "dg C", Attribute::LogOnly, "\
Temperature of water in mulch.");
    frame.declare ("T_soil", "dg C", Attribute::LogOnly, "\
Temperature of top soil.");
    frame.declare ("T_factor", Attribute::None (), Attribute::LogOnly, "\
Temperature effect on turnover.");
    frame.declare ("SMB_C", "g C/cm^3", Attribute::LogOnly, "\
Microbical C in top soil.");
    frame.declare ("SMB_factor", Attribute::None (), Attribute::LogOnly, "\
Microbical effect on turnover.");
    frame.declare ("factor", Attribute::None (), Attribute::LogOnly, "\
The combined effect of SMB, T, h, and contact.\n\
This affects both turnover and chemical decomposition in litter.");
    frame.declare ("DOC_gen", "g/cm^2/h", Attribute::LogOnly, "\
Dissolved organic carbon generated from turnover.");
    frame.declare ("DON_gen", "g/cm^2/h", Attribute::LogOnly, "\
Dissolved organic nitrogen generated from turnover.");
    frame.declare ("SOC_gen", "g/cm^2/h", Attribute::LogOnly, "\
Stationary organic carbon generated from turnover.");
    frame.declare ("SON_gen", "g/cm^2/h", Attribute::LogOnly, "\
Stationary organic nitrogen generated from turnover.");
  }
} LitterMulch_syntax;

// litter.C ends here.
