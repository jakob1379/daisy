#ifndef SURFACE_SIMPLE_H
#define SURFACE_SIMPLE_H

#include "surface.h"

struct SurfaceSimple : Surface
{
  SurfaceSimple(const BlockModel& al)
    : Surface (al)
  { }
  ~SurfaceSimple()
  { }
  void output(Log& log) const override
  { }
};

#endif
