#include "models/surface/surface_simple_model.h"

void SurfaceSimpleModel::accept_top (double amount, const Geometry&, size_t edge, 
                                     double dt, Treelog&)
{ }

// Column.
double SurfaceSimpleModel::runoff_rate () const // [h^-1]
{ return 0.0; }
double SurfaceSimpleModel::mixing_resistance () const // [h/mm]
{ return 1.0e9; }
double SurfaceSimpleModel::mixing_depth () const // [cm]
{ return 0.1; }
  
// Manager.
void SurfaceSimpleModel::set_detention_capacity (double)
{ }

// Simulation.
void SurfaceSimpleModel::update_pond_average (const Geometry& )
{ }
void SurfaceSimpleModel::tick (const Time&, double dt /* [h] */,
                               double PotSoilEvaporationWet, 
                               double PotSoilEvaporationDry, 
                               double flux_in /* [mm/h] */,
                               double temp /* [dg C] */, const Geometry& geo,
                               const Soil&, const SoilWater&,
                               double soil_T /* [dg C] */, Treelog&)
{ T = temp; }

// Communication with bioclimate.
double SurfaceSimpleModel::ponding_average () const // [mm]
{ return 0.0; }
double SurfaceSimpleModel::ponding_max () const     // [mm]
{ return 0.0; }
double SurfaceSimpleModel::temperature () const     // [dg C]
{ return T; }
double SurfaceSimpleModel::EpFactor () const        // []
{ return 0.6; }
double SurfaceSimpleModel::albedo (const Geometry&, const Soil&, const SoilWater&) const
{ return 0.08; }
double SurfaceSimpleModel::exfiltration (double dt) const // [mm/h]
{ return 0.0; }
double SurfaceSimpleModel::evap_soil_surface () const // [mm/h]
{ return 0.0; }
double SurfaceSimpleModel::evap_pond (double dt, Treelog&) const // [mm/h]
{ return 0.0; }
void SurfaceSimpleModel::put_ponding (double pond)	// [mm]
{ }
void SurfaceSimpleModel::set_svat_temperature (double temp /* dg C */)
{ T = temp; }
  
// Create.
void SurfaceSimpleModel::initialize (const Geometry&)
{ }


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

