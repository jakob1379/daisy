#ifndef SURFACE_SOURCE_FLUX_H
#define SURFACE_SOURCE_FLUX_H
#include <memory>
#include "models/surface/surface_simple_model.h"
#include "daisy_time.h"
#include "source.h"
#include "units.h"

struct SurfaceSourceFluxModel : public SurfaceSimpleModel
{
  const Units& units;
  const bool interpolate;	//  Interpolate flux values.
  const std::unique_ptr<Source> source;
  size_t index;
  enum { uninitialized, working, error } state;
  Time previous_time;
  Time next_time;
  double previous_flux;
  double next_flux;
  double flux;

  top_t top_type (const Geometry&, size_t edge) const;
  double q_top (const Geometry&, size_t edge, const double dt) const;
  double h_top (const Geometry& geo, size_t edge) const; // [cm]
  void tick (const Time& time, double dt /* [h] */,
             double PotSoilEvaporationWet, 
             double PotSoilEvaporationDry, 
             double flux_in /* [mm/h] */,
             double temp /* [dg C] */, const Geometry& geo,
             const Soil& soil, const SoilWater& soil_water,
             double soil_T /* [dg C] */,
	     Treelog& msg);
  void tick_source (const Time& time, Treelog& msg);

  // Create.
  SurfaceSourceFluxModel (const Units& units, bool interpolate, Source *source);
  ~SurfaceSourceFluxModel ();
};
#endif
