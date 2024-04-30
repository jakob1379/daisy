#include "block_model.h"
#include "librarian.h"

#include "surface_simple.h"
#include "models/surface/surface_const_flux_model.h"

struct SurfaceConstFluxComponent : SurfaceSimple, SurfaceConstFluxModel
{
  SurfaceConstFluxComponent (const BlockModel& al)
    : SurfaceSimple (al),
      SurfaceConstFluxModel (al.number("flux"))
  { }
  ~SurfaceConstFluxComponent()
  { }
};

static struct SurfaceConstFluxSyntax : DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new SurfaceConstFluxComponent (al); }

  SurfaceConstFluxSyntax () 
    : DeclareModel (SurfaceConstFluxComponent::component, "const_flux", "simple", "\
  Constant flux upper boundary for soil.")
  { }

  void load_frame (Frame& frame) const
  {
    frame.declare ("flux", "cm/h", Attribute::Const, "\
 Flux up from soil.");
    frame.order ("flux");
  }
} SurfaceConstFlux_syntax;

// Surface none
static struct SurfaceNoneSyntax : public DeclareParam
{ 
  SurfaceNoneSyntax ()
    : DeclareParam (SurfaceConstFluxComponent::component, "none", "const_flux", "\
//  Zero flux upper boundary for soil.")
  { }
  void load_frame (Frame& frame) const
  { frame.set ("flux", 0.0); }
} SurfaceNone_syntax;
