#include "models/surface/surface_const_flux_model.h"

SurfaceConstFluxModel::top_t
SurfaceConstFluxModel::top_type (const Geometry&, size_t edge) const
{
  return forced_flux;
}

double SurfaceConstFluxModel::q_top (const Geometry&, size_t edge, const double dt) const // [cm/h]
{
  return flux;
}

double SurfaceConstFluxModel::h_top (const Geometry& geo, size_t edge) const // [cm]
{
  const double dt = 1.0;       // [h]
  return -q_top (geo, edge, dt) * dt; 
}

SurfaceConstFluxModel::SurfaceConstFluxModel (double flux)
  : SurfaceSimpleModel(),
    flux (flux)
{ }

SurfaceConstFluxModel::~SurfaceConstFluxModel ()
{ }
