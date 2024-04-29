#ifndef LITTER_PERMANENT_MODEL_H
#define LITTER_PERMANENT_MODEL_H
#include "models/litter/litter_model.h"

struct LitterPermanentModel : public LitterModel
{
  // Parameters.
  const double vapor_flux_factor_;
  const double interception_capacity;
  const double albedo_;

  // Simulation.
  void tick (const Bioclimate&, const Geometry& geo, const Soil& soil,
	     const SoilWater& soil_water, const SoilHeat& soil_heat,	    
	     OrganicMatter& organic, Chemistry& chemistry,
	     const double dt,
	     Treelog& msg)
  { }
  double cover () const
  { return 1.0; }
  double vapor_flux_factor () const
  { return vapor_flux_factor_; }
  double water_capacity () const
  { return interception_capacity; }
  double albedo () const
  { return albedo_; }

  // Create and Destroy.
  LitterPermanentModel (double vapor_flux_factor,
                        double interception_capacity,
                        double albedo)
    : LitterModel (),
      vapor_flux_factor_ (vapor_flux_factor),
      interception_capacity (interception_capacity),
      albedo_ (albedo)
  { }
  ~LitterPermanentModel ()
  { }
};
#endif
