#include "models/surface/surface_simple_model.h"
#include "surface_simple.h"
#include "block_model.h"
#include "librarian.h"

// Surface simple (base model)
static struct SurfaceSimpleSyntax : DeclareBase
{
  SurfaceSimpleSyntax () 
    : DeclareBase (Surface::component, "simple", "\
  Don't keep track of surface stuff.")
  { }

  void load_frame (Frame&) const
  { }
} SurfaceSimple_syntax;


// Surface const flux 
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
    : DeclareModel (Surface::component, "const_flux", "simple", "\
  Constant flux upper boundary for soil.")
  { }

  void load_frame (Frame& frame) const
  {
    frame.declare ("flux", "cm/h", Attribute::Const, "\
 Flux up from soil.");
    frame.order ("flux");
  }
} SurfaceConstFlux_syntax;



// Surface const pressure
struct SurfaceConstPressureComponent : SurfaceSimple, SurfaceConstPressureModel
{
  SurfaceConstPressureComponent(const BlockModel& al)
    : SurfaceSimple (al),
      SurfaceConstPressureModel (al.number("pressure"))
  { }
  ~SurfaceConstPressureComponent()
  { }
};


static struct SurfaceConstPressureSyntax : DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new SurfaceConstPressureComponent (al); }

  SurfaceConstPressureSyntax () 
    : DeclareModel (Surface::component, "const_pressure", "simple", "\
//  Constant pressure upper boundary for soil.")
  { }

  void load_frame (Frame& frame) const
  {
    frame.declare ("pressure", "cm", Attribute::Const, "\
//  Soil upper boundary pressure.");
    frame.order ("pressure");
  }
} SurfaceConstPressure_syntax;


// Surface none
static struct SurfaceNoneSyntax : public DeclareParam
{ 
  SurfaceNoneSyntax ()
    : DeclareParam (Surface::component, "none", "const_flux", "\
//  Zero flux upper boundary for soil.")
  { }
  void load_frame (Frame& frame) const
  { frame.set ("flux", 0.0); }
} SurfaceNone_syntax;
