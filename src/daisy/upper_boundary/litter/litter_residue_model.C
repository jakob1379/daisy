#include "organic.h"
#include "models/litter/litter_residue_model.h"

void 
LitterResidueModel::tick (const Bioclimate& bioclimate,
		     const Geometry& geo, const Soil& soil,
		     const SoilWater& soil_water, const SoilHeat& soil_heat,
		     OrganicMatter& organic, Chemistry& chemistry,
		     const double dt,
		     Treelog& msg)
{
  mass = organic.top_DM ();
  const double MAI = mass * specific_AI; 
  cover_ = 1.0 - std::exp (- MAI * extinction_coefficent);
}
