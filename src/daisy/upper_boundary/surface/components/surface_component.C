#include "librarian.h"
#include "surface.h"

static struct SurfaceInit : public DeclareComponent 
{
  SurfaceInit ()
    : DeclareComponent (Surface::component, "\
  Surface processes.")
  { }
} Surface_init;
