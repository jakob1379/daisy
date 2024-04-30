#include "models/litter/litter_permanent_model.h"

void LitterPermanentModel::tick (const Bioclimate&,
                                 const Geometry&,
                                 const Soil&,
                                 const SoilWater&,
                                 const SoilHeat&,	    
                                 OrganicMatter&,
                                 Chemistry&,
                                 const double,
                                 Treelog&)
{ }

double LitterPermanentModel::cover () const
{ return 1.0; }

double LitterPermanentModel::vapor_flux_factor () const
{ return vapor_flux_factor_; }

double LitterPermanentModel::water_capacity () const
{ return interception_capacity; }

double LitterPermanentModel::albedo () const
{ return albedo_; }

LitterPermanentModel::LitterPermanentModel (double vapor_flux_factor,
                                            double interception_capacity,
                                            double albedo)
  : LitterModel (),
    vapor_flux_factor_ (vapor_flux_factor),
    interception_capacity (interception_capacity),
    albedo_ (albedo)
{ }

LitterPermanentModel::~LitterPermanentModel ()
{ }

