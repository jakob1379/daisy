#ifndef LITTER_MULCH_MODEL_H
#define LITTER_MULCH_MODEL_H

#include <memory>

#include "plf.h"

#include "retention.h"
#include "models/litter/litter_residue_model.h"

class Log;

struct LitterMulchModel : public LitterResidueModel 
{
  // Parameters.
  const symbol DOC_name;	// Name of chemical representing DOC
  const symbol DON_name;	// Name of chemical representing DON
  const double density;		 // Bulk density of mulch [kg DM/m^3]
  const double particle_density; // Particle density of mulch [kg DM/m^3]
  const double decompose_height; // Max height of active mulch layer [cm]
  const double evaporate_depth;	 // How fow down the litter can evap [cm]
  const double soil_height;	 // Heigh of soil layer affecting decay [cm]
  const double Theta_res;	 // Residual water content []
  const double Theta_sat;	 // Saturated water content []
  const double h_min;		 // Min. pressure for biological activity [cm]
  const double factor_exch;	 // Water connectivity with soil []
  const double alpha;		 // Interception parameter []
  const double Si;		 // Saturation index []
  const std::unique_ptr<Retention> retention; // Retension curve.
  const PLF decompose_heat_factor; // [dg C] -> []
  const double T_scale;		 // Scale to T_ref []
  const PLF decompose_water_factor; // [cm] -> []
  const int decompose_SMB_pool; // 0 = SMB1, 1 = SMB2, -1 = all
  const double decompose_SMB_KM; // MM kin. par. [g C/cm^3]
  const double SMB_scale;	// Scale to SMB_ref []
  const bool use_soil_decompose; // True iff T and h of top soil should be used
  
  // Log variables.
  double height;		// Height of mulch layer [cm]
  double contact;		// Fraction in contact with soil []
  double water;			// Total water in mulch [mm]
  double protected_water;	// Max water protected from evaporation [mm]
  double Theta;			// Relative water content []
  double h;			// Mulch water potential at start of tstep [cm]
  double h1;			// estimated mulch water potential at end [cm]
  double h_soil;		// Soil water potential at soil_height [cm]
  double K_soil;		// Soil water conductivity at soil_height [cm/h]
  double E_darcy;		// Potential exchange with soil water [cm/h]
  double h_factor;		// Water potential effect []
  double T;			// Temperature of mulch [dg C]
  double T_soil;		// Temperature of top soil [dg C]
  double T_factor;		// Temperature factor []
  double SMB_C;			// Microbes in soil [g C/cm^3]
  double SMB_factor;		// Microbial acivity factor []
  double factor;		// contact * h * T * SMB factors []
  double DOC_gen;		// Disolved organic C generation [g C/cm^2/h]
  double DON_gen;		// Disolved organic N generation [g N/cm^2/h]
  double SOC_gen;		// Stationary C generation [g C/cm^2/h]
  double SON_gen;		// Stationary N generation [g N/cm^2/h]


  double find_E_darcy (const double h) const; // [cm/h]
  double find_h1 (const double h0, const double dt) const;
  double find_T_factor_raw (const double T) const;
  double find_T_factor (const double T) const;
  double find_SMB_factor_raw (const double SMB_C) const;
  double find_SMB_factor (const double SMB_C) const;

  // Simulation.
  void tick (const Bioclimate& bioclimate,
	     const Geometry& geo, const Soil& soil,
	     const SoilWater& soil_water, const SoilHeat& soil_heat,
	     OrganicMatter& organic, Chemistry& chemistry,
	     const double dt,
	     Treelog& msg) override;

  double intercept () const;	// [0-1]
  // Fraction of rain hitting the litter layer that actually enter the
  // litter layer.
  bool diffuse () const;
  double potential_exfiltration () const; // Water exchange with soil [mm/h]
  double water_protected () const;    // Water not evapable [mm]
  double decompose_factor () const; // Effect on chemical decomposition []
  void output (Log& log) const;

  // Create and Destroy.
  LitterMulchModel (double water_capacity,
                    double vapor_flux_factor,
                    double specific_AI,
                    double extinction_coefficient,
                    double albedo,
                    symbol DOC_name,
                    symbol DON_name,
                    double density,
                    double particle_density,
                    double decompose_height,
                    double evaporate_depth,
                    double soil_height,
                    double Theta_res,
                    double Theta_sat,
                    double h_min,
                    double factor_exch,
                    double alpha,
                    double Si,
                    Retention *retention,
                    PLF decompose_heat_factor,
                    double T_scale,
                    PLF decompose_water_factor,
                    int decompose_SMB_pool,
                    double decompose_SMB_KM,
                    double SMB_scale,
                    bool use_soil_decompose,
                    Treelog &msg);
  ~LitterMulchModel ();
};
#endif
