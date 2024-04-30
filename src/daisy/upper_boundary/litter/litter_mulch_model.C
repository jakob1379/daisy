#include <sstream>

#include "abiotic.h"
#include "am.h"
#include "aom.h"
#include "bioclimate.h"
#include "chemical.h"
#include "chemistry.h"
#include "geometry.h"
#include "iterative.h"
#include "log.h"
#include "mathlib.h"
#include "organic.h"
#include "smb.h"
#include "soil_heat.h"
#include "soil_water.h"
#include "treelog.h"

#include "models/litter/litter_mulch_model.h"

void LitterMulchModel::output (Log& log) const
{
  LitterModel::output (log);
  output_variable (mass, log);
  output_variable (height, log);
  output_variable (contact, log);
  output_variable (water, log);
  output_variable (protected_water, log);
  output_variable (Theta, log);
  output_variable (h, log);
  output_variable (h1, log);
  output_variable (h_soil, log);
  output_variable (K_soil, log);
  output_variable (E_darcy, log);
  output_variable (h_factor, log);
  output_variable (T, log);
  output_variable (T_soil, log);
  output_variable (T_factor, log);
  output_variable (SMB_C, log);
  output_variable (SMB_factor, log);
  output_variable (factor, log);
  output_variable (DOC_gen, log);
  output_variable (DON_gen, log);
  output_variable (SOC_gen, log);
  output_variable (SON_gen, log);
}

double LitterMulchModel::find_E_darcy (const double h) const // [cm/h]
{ return cover () * factor_exch * ((h - h_soil) / -soil_height) * K_soil; }

double LitterMulchModel::find_h1 (const double h0, const double dt) const
{
  // No litter.
  if (!std::isnormal (height))
    return h_min;

  const double E_darcy = find_E_darcy (h0); // [cm/h]
  const double new_water = water - E_darcy * dt * 10.0 /* [mm/cm] */;
  const double new_Theta
    = Theta_sat * new_water / water_capacity (); // []
  return retention->h (new_Theta);
}

double LitterMulchModel::find_T_factor_raw (const double T) const
{
  if (decompose_heat_factor.size () < 1)
    return Abiotic::f_T0 (T);

  return decompose_heat_factor (T);
}

double LitterMulchModel::find_T_factor (const double T) const
{ return T_scale * find_T_factor_raw (T); }

double LitterMulchModel::find_SMB_factor_raw (const double SMB_C) const
{
  if (decompose_SMB_KM > 0.0)
    return SMB_C / (decompose_SMB_KM + SMB_C);

  return 1.0;
}

double LitterMulchModel::find_SMB_factor (const double SMB_C) const
{
  return SMB_scale * find_SMB_factor_raw (SMB_C);
}


double LitterMulchModel::intercept () const	// [0-1]
// Fraction of rain hitting the litter layer that actually enter the
// litter layer.
{
  if (Theta >= Theta_sat)
    return 0.0;

  return std::exp (-alpha * (Theta_sat - Theta_res) / (Theta_sat - Theta));
}

bool LitterMulchModel::diffuse () const 
{ return Theta >= Si; }
  
double LitterMulchModel::potential_exfiltration () const // Water exchange with soil [mm/h]
{ return E_darcy * 10.0 /* [mm/cm] */; }
  
double LitterMulchModel::water_protected () const    // Water not evapable [mm]
{ return protected_water; }
  
double LitterMulchModel::decompose_factor () const // Effect on chemical decomposition []
{ return factor; }


// Simulation.
void LitterMulchModel::tick (const Bioclimate& bioclimate,
                             const Geometry& geo, const Soil& soil,
                             const SoilWater& soil_water, const SoilHeat& soil_heat,
                             OrganicMatter& organic, Chemistry& chemistry,
                             const double dt,
                             Treelog& msg)
{
  // Find mass and cover.
  LitterResidueModel::tick (bioclimate,
                            geo, soil, soil_water, soil_heat, organic, chemistry,
                            dt, msg);

  // Find height of mulch layer.
  static const double cm_per_m = 100.0;
  if (cover () > 0.0)
    height = cm_per_m * mass / density;
  else
    height = 0.0;

  // Find mass of active part of mulch layer.
  if (decompose_height >= height)
    contact = 1.0;
  else
    contact = decompose_height / height;

  // Water
  water = bioclimate.get_litter_water (); // [mm]
  if (height > 0.0)
    Theta = Theta_sat * water / water_capacity (); // [mm/mm]
  else
    Theta = 0.0;

  const double C = water_capacity (); // [mm]
  const double R = C * Theta_res / Theta_sat;	// [mm]
  const double H = height;		// [cm]
  const double D = evaporate_depth;	// [cm]
  if (H > D)
    protected_water = R + (C - R) * (H - D) / H;
  else
    protected_water = R;
    
  h = retention->h (Theta);

  h_soil = geo.content_height (soil_water, &SoilWater::h, soil_height);
  K_soil = geo.content_height (soil_water, &SoilWater::K_cell, soil_height);

  const double h_decompose = use_soil_decompose ? h_soil : h;
  if (decompose_water_factor.size () < 1)
    h_factor = Abiotic::f_h (h_decompose);
  else
    h_factor = decompose_water_factor (h_decompose);

  struct h_diff_c
  {
    const LitterMulchModel& m;
    const double dt;
    double operator()(const double h0) const
    { return h0 - m.find_h1 (h0, dt); }
    h_diff_c (const LitterMulchModel& m_, const double dt_)
      : m (m_),
        dt (dt_)
    { }
  } h_diff (*this, dt);
  h1 = bisection (h_min, 0, h_diff);
  E_darcy = find_E_darcy (h1);
    
  // Temperature
  T_soil = geo.content_height (soil_heat, &SoilHeat::T, soil_height);
  T = bioclimate.get_litter_temperature (); // [dg C]
  const double T_decompose = use_soil_decompose ? T_soil : T; 
  T_factor = find_T_factor (T_decompose);
 
  // SMB
  const std::vector <SMB*>& smb = organic.get_smb ();
  SMB_C = 0;
  for (int i = 0; i < smb.size (); i++)
    if (i == decompose_SMB_pool || decompose_SMB_pool < 0)
      SMB_C += geo.total_surface (smb[i]->C, 0, soil_height) / -soil_height;
  SMB_factor = find_SMB_factor (SMB_C);

  // Combined
  factor = T_factor * h_factor * SMB_factor * contact;

  // Turnover
  std::vector<AOM*> added;

  for (auto pool: organic.get_am ())
    pool->append_to (added);

  sort (added.begin (), added.end (), AOM::compare_CN);
    
  DOC_gen = 0.0;		// [g C/cm^2/h]
  DON_gen = 0.0;		// [g N/cm^2/h]
  SOC_gen = 0.0;		// [g C/cm^2/h]
  SON_gen = 0.0;		// [g N/cm^2/h]

  if (!std::isnormal (dt))	// Initialization.
    return;
    
  for (auto pool: added)
    {
      const double rate = factor * pool->turnover_rate; // [h^-1]
      const double stationary = pool->SOM_fraction ();  // []
      const double dissolved = 1.0 - stationary;	  // []
      const double top_C = pool->top_C; // [g C/cm^2]

      double new_C = NAN; // [g C/cm^2]
      double loss_C = NAN; // [g C/cm^2/h]
      first_order_change (top_C, 0, rate, dt, new_C, loss_C);
      daisy_assert (loss_C >= 0.0);
      daisy_assert (new_C >= 0.0);
      DOC_gen += loss_C * dissolved;
      SOC_gen += loss_C * stationary;
      pool->top_C = new_C;
	
      const double top_N = pool->top_N; // [g N/cm^2]
      double new_N = NAN; // [g N/cm^2]
      double loss_N = NAN; // [g N/cm^2/h]
      first_order_change (top_N, 0, rate, dt, new_N, loss_N);
      daisy_assert (loss_N >= 0.0);
      daisy_assert (new_N >= 0.0);
      DON_gen += loss_N * dissolved;
      SON_gen += loss_N * stationary;
      pool->top_N = new_N;
    }
  double buffer_C = SOC_gen * dt;
  double buffer_N = SON_gen * dt;

  if (chemistry.know (DOC_name))
    {
      Chemical& DOC = chemistry.find (DOC_name);
      DOC.add_to_litter_transform_source (DOC_gen);
    }
  else
    buffer_C += DOC_gen * dt;

  if (chemistry.know (DON_name))
    {
      Chemical& DON = chemistry.find (DON_name);
      DON.add_to_litter_transform_source (DON_gen);
    }
  else
    buffer_N += DON_gen * dt;

  daisy_assert (buffer_C >= 0.0);
  daisy_assert (buffer_N >= 0.0);
  daisy_assert (dt > 0.0);
  organic.add_to_buffer (geo, 0, soil_height, buffer_C, buffer_N);
}


LitterMulchModel::LitterMulchModel (double water_capacity,
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
                                    Treelog &msg)
: LitterResidueModel (water_capacity,
                      vapor_flux_factor,
                      specific_AI,
                      extinction_coefficient,
                      albedo),
  DOC_name (DOC_name),
  DON_name (DON_name),
  density (density),
  particle_density (particle_density),
  decompose_height (decompose_height),
  evaporate_depth (evaporate_depth),
  soil_height (soil_height),
  Theta_res (Theta_res),
  Theta_sat (Theta_sat),
  h_min (h_min),
  factor_exch (factor_exch),
  alpha (alpha),
  Si (Theta_sat * Si), // This is confusing
  retention (retention),
  decompose_heat_factor (decompose_heat_factor),
  T_scale (T_scale), 
  decompose_water_factor (decompose_water_factor),
  decompose_SMB_pool (decompose_SMB_pool),
  decompose_SMB_KM (decompose_SMB_KM),
  SMB_scale (SMB_scale),
  use_soil_decompose (use_soil_decompose),
  height (NAN),
  contact (NAN),
  water (NAN),
  Theta (NAN),
  h (NAN),
  h1 (NAN),
  h_soil (NAN),
  K_soil (NAN),
  E_darcy (NAN),
  h_factor (NAN),
  T (NAN),
  T_soil (NAN),
  T_factor (NAN),
  SMB_C (NAN),
  SMB_factor (NAN),
  factor (NAN),
  DOC_gen (NAN),
  DON_gen (NAN),
  SOC_gen (NAN),
  SON_gen (NAN)
{
  //TREELOG_MODEL (al.msg ());
  retention->initialize (Theta_res, h_min, Theta_sat, msg);
  std::ostringstream tmp;
  tmp << "Theta_sat = " << Theta_sat
      << "\nT_scale = " << T_scale
      << "\nSMB_scale = " << SMB_scale;
  msg.debug (tmp.str ());
}

LitterMulchModel::~LitterMulchModel ()
{ }
