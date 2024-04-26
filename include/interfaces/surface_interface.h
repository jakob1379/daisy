#ifndef SURFACE_INTERFACE_H
#define SURFACE_INTERFACE_H

#include <cstddef>

class Geometry;
class Log;
class Treelog;
class Soil;
class SoilWater;
class Time;

struct SurfaceInterface
{
  // Communication with soil water.
  enum top_t { forced_pressure, forced_flux, limited_water };
  virtual top_t top_type (const Geometry&, size_t edge) const = 0;
  virtual double q_top (const Geometry&, size_t edge, const double dt) const = 0; // [cm/h]
  virtual double h_top (const Geometry&, size_t edge) const = 0; // [cm]
  virtual void accept_top (double amount, const Geometry&, size_t edge, 
			   double dt, Treelog&) = 0;

  // Column.
  virtual double runoff_rate () const = 0; // [h^-1]
  virtual double mixing_resistance () const = 0; // [h/mm]
  virtual double mixing_depth () const = 0; // [cm]
  
  // Manager.
  virtual void set_detention_capacity (double) = 0;

  // Simulation.
  virtual void update_pond_average (const Geometry& geo) = 0;
  virtual void tick (const Time&, double dt /* [h] */, 
		     double PotSoilEvaporationWet, 
		     double PotSoilEvaporationDry, 
		     double flux_in /* [mm/h] */,
		     double temp /* [dg C] */, const Geometry& geo,
		     const Soil&, const SoilWater&,
		     double soil_T /* [dg C] */, Treelog&) = 0;

  // Communication with bioclimate.
  virtual double ponding_average () const = 0; // [mm]
  virtual double ponding_max () const = 0;     // [mm]
  virtual double temperature () const = 0;     // [dg C]
  virtual double EpFactor () const = 0;        // []
  virtual double albedo (const Geometry&, const Soil&, const SoilWater&) const = 0;
  virtual double exfiltration (double dt) const = 0; // [mm/h]
  virtual double evap_soil_surface () const = 0; // [mm/h]
  virtual double evap_pond (double dt, Treelog&) const = 0; // [mm/h]
  virtual void put_ponding (double pond) = 0;	// [mm]
  virtual void set_svat_temperature (double T /* dg C */) = 0;
  
  // Create.
  virtual void initialize (const Geometry&) = 0;
  virtual ~SurfaceInterface () {};
};

#endif // SURFACE_INTERFACE_H
