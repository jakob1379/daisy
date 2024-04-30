#ifndef SURFACE_CONST_FLUX_MODEL_H
#define SURFACE_CONST_FLUX_MODEL_H

#include "models/surface/surface_simple_model.h"

struct SurfaceConstFluxModel : public SurfaceSimpleModel
{
  // Content.
  const double flux;

  top_t top_type (const Geometry&, size_t edge) const;
  double q_top (const Geometry&, size_t edge, const double dt) const; // [cm/h]
  double h_top (const Geometry& geo, size_t edge) const; // [cm]

  // Create.
  SurfaceConstFluxModel (double flux);
  ~SurfaceConstFluxModel ();
};
#endif
