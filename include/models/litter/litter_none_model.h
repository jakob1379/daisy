#ifndef LITTER_NONE_MODEL_H
#define LITTER_NONE_MODEL_H

#include "models/litter/litter_model.h"

struct LitterNoneModel : public LitterModel
{
  // Simulation.
  void tick (const Bioclimate&, const Geometry& geo, const Soil& soil,
	     const SoilWater& soil_water, const SoilHeat& soil_heat,
	     OrganicMatter& organic, Chemistry& chemistry,
	     const double dt,
	     Treelog& msg)
  { }
  double cover () const
  { return 0.0; }
  double vapor_flux_factor () const
  { return 1.0; }
  double water_capacity () const
  { return 0.0; }
  double albedo () const
  { return -1.0; }

  // Create and Destroy.
  LitterNoneModel ()
    : LitterModel()
  { }
  ~LitterNoneModel ()
  { }
};
#endif
