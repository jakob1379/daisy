#include "log.h"

#include "models/litter/litter_none_model.h"

// Simulation.
void LitterNoneModel::tick (const Bioclimate&,
                            const Geometry&,
                            const Soil&,
                            const SoilWater&,
                            const SoilHeat&,
                            OrganicMatter&,
                            Chemistry&,
                            const double,
                            Treelog&)
{ }
  
double LitterNoneModel::cover () const
{ return 0.0; }
  
double LitterNoneModel::vapor_flux_factor () const
{ return 1.0; }
  
double LitterNoneModel::water_capacity () const
{ return 0.0; }
  
double LitterNoneModel::albedo () const
{ return -1.0; }

LitterNoneModel::LitterNoneModel ()
  : LitterModel()
{ }
  
LitterNoneModel::~LitterNoneModel ()
{ }


