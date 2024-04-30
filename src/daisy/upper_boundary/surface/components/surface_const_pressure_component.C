#include "block_model.h"
#include "librarian.h"

#include "surface.h"
#include "models/surface/surface_const_pressure_model.h"

struct SurfaceConstPressureComponent : Surface, SurfaceConstPressureModel
{
  void output (Log& log) const
  {
    SurfaceConstPressureModel::output (log);
  }

  SurfaceConstPressureComponent(const BlockModel& al)
    : Surface (al),
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
    : DeclareModel (SurfaceConstPressureComponent::component, "const_pressure", "simple", "\
//  Constant pressure upper boundary for soil.")
  { }

  void load_frame (Frame& frame) const
  {
    frame.declare ("pressure", "cm", Attribute::Const, "\
//  Soil upper boundary pressure.");
    frame.order ("pressure");
  }
} SurfaceConstPressure_syntax;
