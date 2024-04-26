#ifndef SURFACE_STANDARD_MODEL_H
#define SURFACE_STANDARD_MODEL_H

#include <cmath>
#include <map>
#include <vector>

#include "plf.h"
#include "interfaces/surface_interface.h"

class Geometry;
class Soil;
class SoilWater;
class Time;
class TreeLog;

struct SurfaceStandardModel : public virtual SurfaceInterface
{
  // Content.
  const double temperature_change_rate; // [h^-1]
  const double EpFactor_;		// []
  const PLF EpFactor_SWE;	// [pF] -> []
  double EpFactor_current;	// []
  const double albedo_wet;
  const double albedo_dry;
#ifdef FORCED_BOUNDARY
  const bool use_forced_pressure;
  const double forced_pressure_value;
  const bool use_forced_flux;
  const double forced_flux_value;
#endif // FORCED_FLUX_VALUE
  typedef std::map<size_t, size_t> pond_map;
  pond_map pond_edge;
  double pond_average;          // [mm]
  double pond_max;              // [mm]
  std::vector<double> pond_section; // [mm]
  double EvapSoilSurface;       // [mm/h]
  double Eps;                   // [mm/h]
  double T;                     // [dg C]
  double DetentionCapacity;     // [mm]
  const double ReservoirConstant; // [h^-1]
  const double LocalDetentionCapacity; // [mm]
  double runoff;                // [mm/h]
  double runoff_rate_;          // [h^-1]
  const double R_mixing;
  const double z_mixing;
  bool print_runoff_rate_;
public:
  // Communication with soil water.
  top_t top_type (const Geometry&, size_t edge) const;
  double q_top (const Geometry&, size_t edge, const double dt) const; // [cm/h]
  double h_top (const Geometry&, size_t edge) const; // [cm]
  void accept_top (double amount, const Geometry&, size_t edge, 
                   double dt, Treelog&);

  // Column.
  double runoff_rate () const; // [h^-1]
  double mixing_resistance () const; // [h/mm]
  double mixing_depth () const; // [cm]
  
  // Manager.
  void set_detention_capacity (double);

  // Simulation.
  void update_pond_average (const Geometry& geo);
  void tick (const Time&, double dt /* [h] */, 
             double PotSoilEvaporationWet, 
             double PotSoilEvaporationDry, 
             double flux_in /* [mm/h] */,
             double temp /* [dg C] */, const Geometry& geo,
             const Soil&, const SoilWater&,
             double soil_T /* [dg C] */,
	     Treelog&);

  // Communication with bioclimate.
  double ponding_average () const; // [mm]
  double ponding_max () const;     // [mm]
  double temperature () const;     // [dg C]
  double EpFactor () const;        // []
  double albedo (const Geometry&, const Soil&, const SoilWater&) const;
  double exfiltration (double dt) const; // [mm/h]
  double evap_soil_surface () const; // [mm/h]
  double evap_pond (double dt, Treelog&) const; // [mm/h]
  void put_ponding (double pond);	// [mm]
  void set_svat_temperature (double T /* dg C */);
  
  // Create.
  void initialize (const Geometry&);
  SurfaceStandardModel (double temperature_change_rate,
                        double EpFactor,
                        PLF EpFactor_SWE,
                        double albedo_wet,
                        double albedo_dry,
#ifdef FORCED_BOUNDARY
                        bool use_forced_pressure,
                        double forced_pressure_value,
                        bool use_forced_flux,
                        double forced_flux_value,
#endif // FORCED_BOUNDARY
                        std::vector<double> pond_section,
                        double DetentionCapacity,
                        double ReservoirConstant,
                        double LocalDetentionCapacity,
                        double R_mixing,
                        double z_mixing)
    : temperature_change_rate (temperature_change_rate),
      EpFactor_ (EpFactor),
      EpFactor_SWE (EpFactor_SWE),
      EpFactor_current (EpFactor_),
      albedo_wet (albedo_wet),
      albedo_dry (albedo_dry),
#ifdef FORCED_BOUNDARY
      use_forced_pressure (use_forced_pressure),
      forced_pressure_value (forced_pressure),
      use_forced_flux (use_forced_flux),
      forced_flux_value (forced_flux),
#endif // FORCED_BOUNDARY
      pond_average (NAN),
      pond_section (pond_section),
      EvapSoilSurface (0.0),
      Eps (0.0),
      T (0.0),
      DetentionCapacity (DetentionCapacity),
      ReservoirConstant (ReservoirConstant),
      LocalDetentionCapacity (LocalDetentionCapacity),
      runoff (0.0),
      runoff_rate_ (0.0),
      R_mixing (R_mixing),
      z_mixing (z_mixing),
      print_runoff_rate_ (true)
  { }
  
  ~SurfaceStandardModel ()
  { }
};
#endif // SURFACE_STANDARD_MODEL_H
