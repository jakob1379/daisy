#include <cmath>
#include <vector>

#include "block_model.h"
#include "check.h"
#include "librarian.h"
#include "log.h"

#include "surface.h"
#include "models/surface/surface_standard_model.h"

struct SurfaceStandardComponent : Surface, SurfaceStandardModel
{
  void output (Log& log) const
  {
    SurfaceStandardModel::output (log);
  }

  SurfaceStandardComponent(const BlockModel& al)
      : Surface (al),
        SurfaceStandardModel(al.number ("temperature_change_rate", -1.0),
                             al.number ("EpFactor"),
                             al.plf ("EpFactor_SWE"),
                             al.number ("albedo_wet"),
                             al.number ("albedo_dry"),
#ifdef FORCED_BOUNDARY
                             al.check ("forced_pressure"),
                             al.number ("forced_pressure", -42.42e42),
                             al.check ("forced_flux"),
                             al.number ("forced_flux", -42.42e42),
#endif // FORCED_BOUNDARY
                             al.check ("pond_section")
                             ? al.number_sequence ("pond_section")
                             : std::vector<double> (),
                             al.number ("DetentionCapacity"),
                             al.number ("ReservoirConstant"),
                             al.number ("LocalDetentionCapacity"),
                             al.number ("R_mixing"),
                             al.number ("z_mixing"))
  { }
  ~SurfaceStandardComponent()
  { }
};

static struct SurfaceStandardSyntax : DeclareModel
{
  bool used_to_be_a_submodel () const
  { return true; }

  Model* make (const BlockModel& al) const
  { return new SurfaceStandardComponent (al); }

  SurfaceStandardSyntax () 
    : DeclareModel (SurfaceStandardComponent::component, "default", "\
Keep track of soil surface.")
  { }

  static bool
  check_alist (const Metalib&, const Frame& al, Treelog& msg)
  {
    bool ok = true;

#ifdef FORCED_BOUNDARY
    if (al.check ("forced_flux") && al.check ("forced_pressure"))
      {
	msg.error ("Can't have both 'forced_pressure' and 'forced_flux'");
	ok = false;
      }
#endif // FORCED_BOUNDARY
    return ok;
  }

  void load_frame (Frame& frame) const
  {
    Model::load_model (frame);
    frame.add_check (check_alist);
    frame.declare ("temperature_change_rate", "h^-1",
		   Attribute::Const,
		   "Relative change of surface temperature.\n\
If you set this to a negative value, surface temperature will\n\
reflect temperature of incomming water or ponded water, if any,\n\
snow, if any, or air temperature.\n\
Set this parameter to a positive value to dampen changes.\n\
Especially useful if you feed the model with daily weather data\n\
and average temperature.");
    frame.set ("temperature_change_rate", 0.5);
    frame.declare ("EpFactor", Attribute::None (), Check::non_negative (), 
		   Attribute::Const,
		   "Convertion of reference evapotranspiration to\n\
potential evaporation for bare soil.");
    frame.set_cited ("EpFactor", 0.6, "\
See figure 4 in the cited paper.\n\
\n\
The autumn value can be lower, due to muching.  With a crop factor of\n\
1.2 a combined Kc of 1.15 is reached at LAI=5.",
		     "kjaersgaard2008crop");
    frame.declare ("EpFactor_SWE",
		   "pF", Attribute::None (), Attribute::Const, "\
Effect of soil water on EpFactor.");
    frame.set ("EpFactor_SWE", PLF::always_1 ());
    frame.declare ("albedo_dry", Attribute::None (), Check::non_negative (),
		   Attribute::Const,
		   "Albedo of dry soil (pF >= 3)");
    frame.set ("albedo_dry", 0.15);
    frame.declare ("albedo_wet", Attribute::None (), Check::non_negative (),
		   Attribute::Const,
		   "Albedo of wet soil (pf <= 1.7)");
    frame.set ("albedo_wet", 0.08);
#ifdef FORCED_BOUNDARY
    frame.declare ("forced_pressure", "mm", Attribute::OptionalConst, "\
Set this to force a permanent pressure top.");
    frame.declare ("forced_flux", "mm/h", Attribute::OptionalConst, "\
Set this to force a permanent flux top.  Positive upwards (exfiltration).");
#endif // FORCED_BOUNDARY
    frame.declare ("pond", "mm", Attribute::LogOnly, "\
Amount of ponding on the surface.\n\
Negative numbers indicate soil exfiltration.");
    frame.declare ("pond_section", "mm",
		   Attribute::OptionalState, Attribute::Variable, "\
Amount of ponding on each section of the surface.\n\
By default, there will be no ponding. In an 1D simulation, there\n\
will only be one section.  In general, there will be a section for each\n\
numeric cell in the soil matrix with an edge towards the surface.");
    frame.declare ("EvapSoilSurface", "mm/h", Attribute::LogOnly, "\
Water evaporated from the surface, including the pond and exfiltration.");
    frame.declare ("Eps", "mm/h", Attribute::LogOnly, "\
Potential evaporation from the surface.");
    frame.declare ("T", "dg C", Attribute::LogOnly, "\
Temperature of water or air directly above the surface.");
    frame.declare ("DetentionCapacity", "mm", Check::non_negative (),
		   Attribute::State, "Amount of ponding the surface can retain.\n\
If ponding in any part of the surface is above this, exceed will runoff.");
    frame.set ("DetentionCapacity", 1000.0);
    frame.declare ("ReservoirConstant", "h^-1", Check::positive (), 
		   Attribute::Const, "\
Fraction of ponding above DetentionCapacity that runoffs each hour.");
    frame.set ("ReservoirConstant", 1.0);
    frame.declare ("LocalDetentionCapacity", "mm", Check::non_negative (),
		   Attribute::State, "\
Amount of ponding the surface can retain locally.\n                     \
If ponding in any part of the surface is above this, exceed will be\n\
distributed to the rest of the surface.");
    frame.set ("LocalDetentionCapacity", 10.0);
    frame.declare ("runoff", "mm/h", Attribute::LogOnly, "\
Amount of water runoff from ponding this hour.");
    frame.declare ("z_mixing", "cm", Check::non_negative (), Attribute::Const, "\
Depth of mixing layer in the top of the soil.\n\
The mixing layer affect exchange between soil coloids, soil water\n\
and the surface, especially in connection with intense rainfall.");
    frame.set ("z_mixing", 0.1);
    frame.declare ("R_mixing", "h/mm", Check::non_negative (), Attribute::Const, "\
Resistance to mixing inorganic compounds between soil and ponding.");
    frame.set ("R_mixing", 1.0e9);
  }
} SurfaceStandard_syntax;
