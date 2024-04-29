#include "block_model.h"
#include "check.h"
#include "librarian.h"
#include "litter.h"
#include "models/litter/litter_permanent_model.h"

struct LitterPermanentComponent : Litter, LitterPermanentModel
{
  LitterPermanentComponent (const BlockModel& al)
    : Litter (al),
      LitterPermanentModel(al.number ("vapor_flux_factor"),
                           al.number ("interception_capacity"),
                           al.number ("albedo", -1.0))
  { }
};

static struct LitterPermanentSyntax : DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new LitterPermanentComponent (al); }
  LitterPermanentSyntax ()
    : DeclareModel (Litter::component, "permanent", "\
A permanent litter layer cover the ground, as for example in a forest.")
  { }
  void load_frame (Frame& frame) const
  { 
    frame.declare_fraction ("vapor_flux_factor", Attribute::Const, "\
Reduction factor for potential evaporation below litter.");
    frame.set ("vapor_flux_factor", 1.0);
    frame.declare ("interception_capacity", "mm", Attribute::Const,
                   "Storage capacity of litter.");
    frame.declare ("albedo", Attribute::None (), Check::positive (),
                   Attribute::OptionalConst, "Reflection factor.\n\
By default, the surface albedo will be used.");
  }
} LitterPermanent_syntax;
