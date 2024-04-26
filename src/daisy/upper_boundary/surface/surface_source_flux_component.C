#include "models/surface/surface_source_flux_model.h"
#include "surface_simple.h"
#include "block_model.h"
#include "librarian.h"

struct SurfaceSourceFluxComponent : SurfaceSimple, SurfaceSourceFluxModel
{
  SurfaceSourceFluxComponent(const BlockModel& al)
    : SurfaceSimple (al),      
      SurfaceSourceFluxModel (al.units (),
                              al.flag ("interpolate"),
                              Librarian::build_item<Source> (al, "source"))
  { }
  ~SurfaceSourceFluxComponent()
  { }
};

static struct SurfaceSourceFluxSyntax : DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new SurfaceSourceFluxComponent (al); }

  SurfaceSourceFluxSyntax () 
    : DeclareModel (SurfaceSourceFluxComponent::component, "source_flux", "simple", "\
Dynamic flux upper boundary for soil.")
  { }

  void load_frame (Frame& frame) const
  {
    frame.declare_object ("source", Source::component, Attribute::Const, 
                          Attribute::Singleton, "\
Flux time series.");
    frame.declare_boolean ("interpolate", Attribute::Const, "\
If true, interpolate values.  If false, use last read.");
    frame.set ("interpolate", false);
  }
} SurfaceSourceFlux_syntax;

