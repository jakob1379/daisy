#include "block_model.h"
#include "librarian.h"

#include "surface_simple.h"
#include "models/surface/surface_simple_model.h"

// Surface simple (base model)
static struct SurfaceSimpleSyntax : DeclareBase
{
  SurfaceSimpleSyntax () 
    : DeclareBase (SurfaceSimple::component, "simple", "\
  Don't keep track of surface stuff.")
  { }

  void load_frame (Frame&) const
  { }
} SurfaceSimple_syntax;
