#include "models/surface/surface_const_pressure_model.h"

SurfaceConstPressureModel::top_t
SurfaceConstPressureModel::top_type (const Geometry&, size_t edge) const
{
  return forced_pressure;
}

double SurfaceConstPressureModel::q_top (const Geometry& geo, size_t edge,
		const double dt) const // [cm/h]
{
  return h_top (geo, edge) / dt;
}

double SurfaceConstPressureModel::h_top (const Geometry&, size_t edge) const // [cm]
{
  return pressure;
}

SurfaceConstPressureModel::SurfaceConstPressureModel (double pressure)
  : SurfaceSimpleModel(),
    pressure (pressure)
{ }

SurfaceConstPressureModel::~SurfaceConstPressureModel ()
{ }
