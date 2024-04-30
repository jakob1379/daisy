#include "log.h"
#include "organic.h"

#include "models/litter/litter_residue_model.h"

void LitterResidueModel::output (Log& log) const
{
  LitterModel::output (log);
  output_variable (mass, log);
}


void 
LitterResidueModel::tick (const Bioclimate&,
                          const Geometry&,
                          const Soil&,
                          const SoilWater&,
                          const SoilHeat&,
                          OrganicMatter& organic,
                          Chemistry&,
                          const double,
                          Treelog&)
{
  mass = organic.top_DM ();
  const double MAI = mass * specific_AI; 
  cover_ = 1.0 - std::exp (- MAI * extinction_coefficent);
}

double LitterResidueModel::cover () const
{ return cover_; }

double LitterResidueModel::vapor_flux_factor () const
{ return vapor_flux_factor_; }

double LitterResidueModel::water_capacity () const
{ return mass * water_capacity_; }

double LitterResidueModel::albedo () const
{ return albedo_; }

LitterResidueModel::LitterResidueModel (double water_capacity,
                                        double vapor_flux_factor,
                                        double specific_AI,
                                        double extinction_coefficent,
                                        double albedo)
  : LitterModel(),
    water_capacity_ (water_capacity),
    vapor_flux_factor_ (vapor_flux_factor),
    specific_AI (specific_AI),
    extinction_coefficent (extinction_coefficent),
    albedo_ (albedo),
    mass (NAN),
    cover_ (NAN)
{ }

LitterResidueModel::~LitterResidueModel ()
{ }
