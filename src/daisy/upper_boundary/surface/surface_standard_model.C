// surface_std.C
// 
// Copyright 1996-2001 Per Abrahamsen and Søren Hansen
// Copyright 2000-2001 KVL.
// Copyright 2022 UCPH
//
// This file is part of Daisy.
// 
// Daisy is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
// 
// Daisy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser Public License for more details.
// 
// You should have received a copy of the GNU Lesser Public License
// along with Daisy; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#define BUILD_DLL

// The 'default' model.
#include <memory>
#include <sstream>

#include "geometry1d.h"
#include "log.h"
#include "mathlib.h"
#include "soil.h"
#include "soil_water.h"
#include "treelog.h"

#include "models/surface/surface_standard_model.h"

void SurfaceStandardModel::output (Log& log) const
{
    output_variable (T, log);
    output_value (pond_average, "pond", log);
    output_variable (pond_section, log);
    output_variable (EvapSoilSurface, log);
    output_variable (Eps, log);
    output_variable (runoff, log);
  }


SurfaceStandardModel::top_t 
SurfaceStandardModel::top_type (const Geometry& geo, size_t edge) const
{
#ifdef FORCED_BOUNDARY
  if (use_forced_flux)
    return forced_flux;

  if (use_forced_pressure)
    return forced_pressure;
#endif // FORCED_BOUNDARY  
  daisy_assert (geo.edge_to (edge) == Geometry::cell_above);
  pond_map::const_iterator i = pond_edge.find (edge);
  daisy_assert (i != pond_edge.end ());
  const size_t c = (*i).second;
  if (pond_section[c] <= 0.0)
    return forced_flux;
  
  return limited_water;
}

double 
SurfaceStandardModel::q_top (const Geometry& geo, const size_t edge,
			const double dt) const
{
#ifdef FORCED_BOUNDARY
  if (use_forced_pressure)
    return -forced_pressure_value * 0.1 / 1.0; // mm -> cm/h.

  if (use_forced_flux)
    return forced_flux_value * 0.1; // mm/h -> cm/h.
#endif // FORCED_BOUNDARY
  
  daisy_assert (geo.edge_to (edge) == Geometry::cell_above);
  pond_map::const_iterator i = pond_edge.find (edge);
  daisy_assert (i != pond_edge.end ());
  const size_t c = (*i).second;
  daisy_assert (pond_section[c] < std::max (1000.0, 10.0 * DetentionCapacity));
  daisy_assert (pond_section[c] > -1000.0);
  return -pond_section[c] * 0.1 / dt /* [h] */; // mm -> cm/h.
}
  
double
SurfaceStandardModel::h_top (const Geometry& geo, size_t edge) const
{ 
  const double dt = 1.0;       // [h]
  return -q_top (geo, edge, dt) * dt; 
}

void
SurfaceStandardModel::accept_top (double water /* [cm] */,
			     const Geometry& geo, size_t edge,
			     double dt, Treelog& msg)
{ 
#ifdef FORCED_BOUNDARY
  if (use_forced_pressure)
    return;
#endif // FORCED_BOUNDARY

  water *= 10.0;		// [cm] -> [mm]

  daisy_assert (geo.edge_to (edge) == Geometry::cell_above);
  const size_t c = pond_edge[edge];
  daisy_assert (pond_section[c] < std::max(1000.0, 10.0 * DetentionCapacity));
  daisy_assert (pond_section[c] > -1000.0);
  pond_section[c] += water;
  daisy_assert (pond_section[c] < std::max (1000.0, 10.0 * DetentionCapacity));
  daisy_assert (pond_section[c] > -1000.0);
}

double
SurfaceStandardModel::ponding_average () const
{ return pond_average; }

double
SurfaceStandardModel::ponding_max () const
{ return pond_max; }

double
SurfaceStandardModel::runoff_rate () const
{ return runoff_rate_; }

double
SurfaceStandardModel::mixing_resistance () const
{ return R_mixing; }

double
SurfaceStandardModel::mixing_depth () const
{ return z_mixing; }

double
SurfaceStandardModel::temperature () const
{ return T; }

void
SurfaceStandardModel::update_pond_average (const Geometry& geo)
{
  const std::vector<size_t>& top_edges = geo.cell_edges (Geometry::cell_above);
  const size_t top_edges_size = top_edges.size ();

  // Find total pond.
  double total_pond = 0.0;      // [mm cm^2]
  double total_area = 0.0;      // [cm^2]
  pond_max = 0.0;
  for (int i = 0; i < top_edges_size; i++)
    {
      const size_t edge = top_edges[i];
      const size_t c = pond_edge[edge];
      const double area = geo.edge_area (edge);
      total_area += area;
      total_pond += pond_section[c] * area;
      if (pond_section[c] > pond_max)
        pond_max = pond_section[c];
    }
  daisy_approximate (total_area, geo.surface_area ());
  pond_average = total_pond / total_area; // [mm];
}

void
SurfaceStandardModel::tick (const Time&, const double dt,
		       const double PotSoilEvaporationWet,
		       const double PotSoilEvaporationDry,
		       const double flux_in, const double temp,
		       const Geometry& geo,
		       const Soil& soil, const SoilWater& soil_water,
		       const double soil_T,
		       Treelog& msg)
{
  // Runoff out of field.
  const double old_pond_average = pond_average;
  runoff = 0.0;
  runoff_rate_ = 0.0;
  double total_area = 0.0;
  for (pond_map::iterator i = pond_edge.begin ();
       i != pond_edge.end ();
       i++)
    {
      const double runoff_speed = // [h^-1]
        (ReservoirConstant * dt > 1.0)
        ? 1.0 / dt
        : ReservoirConstant;
      const size_t edge = (*i).first;
      const size_t c = (*i).second;
      const double area = geo.edge_area (edge);
      total_area += area;
      if (pond_section[c] > DetentionCapacity)
        {
          const double runoff_section // [mm/h]
            = (pond_section[c] - DetentionCapacity) * runoff_speed;
          runoff += area * runoff_section; // [A mm/h]
          runoff_rate_ +=          // [A h^-1]
            area * runoff_section / pond_section[c]; 
          pond_section[c] -= runoff_section * dt; // [mm]
        }
    }
  runoff /= total_area;         // [mm]
  runoff_rate_ /= total_area; // [h^-1]
  update_pond_average (geo);
  daisy_balance (old_pond_average, pond_average, -runoff * dt);

  // Runoff internal.
  const double local_pond_average = pond_average;
  if (pond_average > LocalDetentionCapacity)
    {
      for (pond_map::iterator i = pond_edge.begin ();
           i != pond_edge.end ();
           i++)
        {
          const size_t c = (*i).second;
          pond_section[c] = pond_average;
        }
      update_pond_average (geo);
    }
  else
    {
      // Any sections above local capacity?
      double extra = 0.0;       // [mm cm^2] 
      double total_area = 0.0;        // [cm^2]
      double free_area = 0.0;         // [cm^2]
      const double epsilon = 1e-9; // [mm]
      for (pond_map::iterator i = pond_edge.begin ();
           i != pond_edge.end ();
           i++)
        {
          const size_t edge = (*i).first;
          const size_t c = (*i).second;
          const double area = geo.edge_area (edge);
          total_area += area;
          if (pond_section[c] > LocalDetentionCapacity + epsilon)
            {
              extra += area * (pond_section[c] - LocalDetentionCapacity);
              pond_section[c] = LocalDetentionCapacity;
            }
          else if (pond_section[c] + epsilon < LocalDetentionCapacity)
            free_area += area;
        }
      update_pond_average (geo);
      daisy_balance (local_pond_average, pond_average, -extra / total_area);

      // Redistribute.
      while (extra > epsilon * total_area && free_area > 1e-9)
        {
          const double old_extra = extra;
          const double fill = extra / free_area;
          free_area = 0.0;
          for (pond_map::iterator i = pond_edge.begin ();
               i != pond_edge.end ();
               i++)
            {
              const size_t edge = (*i).first;
              const size_t c = (*i).second;
              const double area = geo.edge_area (edge);
              
              if (pond_section[c] + fill + epsilon < LocalDetentionCapacity)
                {
                  extra -= fill * area;
                  pond_section[c] += fill;
                  free_area += area;
                }
              else if (pond_section[c] < LocalDetentionCapacity)
                {
                  extra -= (LocalDetentionCapacity - pond_section[c]) * area;
                  pond_section[c] = LocalDetentionCapacity;
                }
            }
          daisy_assert (extra < old_extra);
        }
    }
  update_pond_average (geo);
  daisy_approximate (local_pond_average, pond_average);
  
  // Remember potential evaporation
  Eps = PotSoilEvaporationWet;

  double EvapSoilTotal = 0.0;   // [mm cm^2/h]

  // Update pond above each top cell.
  const std::vector<size_t>& top_edges = geo.cell_edges (Geometry::cell_above);
  const size_t top_edges_size = top_edges.size ();
  for (size_t i = 0; i < top_edges_size; i++)
    {
      const size_t edge = top_edges[i];
      const size_t c = pond_edge[edge];
      const double area = geo.edge_area (edge); // [cm^2]

      // Exfiltration.
      const double MaxExfiltration // [mm/h]
        = bound (0.0, 
                 soil_water.MaxExfiltration (geo, edge, soil, soil_T) * 10.0,
                 PotSoilEvaporationDry); 

      const double epond = pond_section[c]; // [mm]

      double evap;              // [mm/h]
      if (epond + flux_in * dt + MaxExfiltration * dt < Eps * dt)
        evap = epond / dt + flux_in + MaxExfiltration;
      else
        evap = Eps;

      EvapSoilTotal += evap * area;    // [mm cm^2/h]

      pond_section[c] += flux_in * dt - evap * dt;
      daisy_assert (pond_section[c]
                    < std::max (1000.0, 10.0 * DetentionCapacity));
      daisy_assert (pond_section[c] > -1000.0);
      daisy_assert (evap < 1000.0);
    }
  EvapSoilSurface = EvapSoilTotal / geo.surface_area (); // [mm/h]
  update_pond_average (geo);

  // Temperature
  double new_T = T;
  if (old_pond_average < 1e-6)
    new_T = temp;
  else if (flux_in < 0.0)
    {
      if (old_pond_average - EvapSoilSurface * dt + flux_in * dt < 1e-6)
        new_T = temp;
      // else use old temperature.
    }
  else
    new_T = (T * old_pond_average + temp * flux_in * dt)
      / (old_pond_average + flux_in * dt);

  // Slow down changes to surface temperature.
  if (temperature_change_rate > 0.0)
    T += (new_T - T) * std::min (temperature_change_rate * dt, 1.0);
  else 
    T = new_T;

  daisy_assert (T > -100.0 && T < 50.0);

  // Adjust EpFactor after soil water.
  const double h
    = geo.content_hood (soil_water, &SoilWater::h, Geometry::cell_above);
  EpFactor_current = (h < 0.0)
    ? EpFactor_ * EpFactor_SWE (h2pF (h))
    : EpFactor_ * EpFactor_SWE (-99.99);
}

double 
SurfaceStandardModel::EpFactor () const
{ return EpFactor_current; }

double
SurfaceStandardModel::albedo (const Geometry& geo, const Soil& soil,
			 const SoilWater& soil_water) const
{ 
  double Theta_pf_3 = 0.0;
  double Theta_pf_1_7 = 0.0;
  double Theta = 0.0; 
  double volume = 0.0;
  
  const size_t cell_size = geo.cell_size ();
  for (size_t i = 0; i < cell_size; i++)
    if (geo.contain_z (i, 0.0))
      {
        const double v = geo.cell_volume (i);
        volume += v;
        Theta_pf_3 += soil_water.Theta_ice (soil, i, pF2h (3.0)) * v;
        Theta_pf_1_7 += soil_water.Theta_ice (soil, i, pF2h (1.7)) * v;
        Theta += soil_water.Theta (i) * v;
      }
  daisy_assert (volume > 0.0);
  Theta_pf_3 /= volume;
  Theta_pf_1_7 /= volume;
  Theta /= volume;

  daisy_assert (Theta_pf_1_7 >= Theta_pf_3);

  if (Theta <= Theta_pf_3)
    return albedo_dry;
  if (Theta > Theta_pf_1_7)
    return albedo_wet;

  if (approximate (Theta_pf_1_7, Theta_pf_3))
    return albedo_wet;

  return albedo_dry + (albedo_wet - albedo_dry)
    * (Theta - Theta_pf_3) / (Theta_pf_1_7 - Theta_pf_3);
}

void
SurfaceStandardModel::set_detention_capacity (const double height)
{ DetentionCapacity = height; }

double
SurfaceStandardModel::exfiltration (const double dt) const // [mm/h]
{
  // Negative pond == amount extracted from soil.
  if (pond_average < 0.0)
    return -pond_average / dt;
  else
    return 0.0;
}

double
SurfaceStandardModel::evap_soil_surface () const // [mm/h]
{ return EvapSoilSurface; }

double 
SurfaceStandardModel::evap_pond (const double dt, Treelog& msg) const // [mm/h]
{ 
  const double ep = evap_soil_surface () - exfiltration (dt); 

  if (ep >= 0.0)
    return ep;
  if (ep < -1e-5)		// ca = 0.1 mm/y
    {
      Treelog::Open nest (msg, "Surface evap pond");
      std::ostringstream tmp;
      tmp << "evap_pond = " << ep << ", evap_soil_surface = " << evap_soil_surface () << ", exfiltration = " << exfiltration (dt);
      msg.warning (tmp.str ());
    }
  return 0.0;
}

void
SurfaceStandardModel::put_ponding (double p)	// [mm]
{ 
  for (pond_map::iterator i = pond_edge.begin ();
       i != pond_edge.end ();
       i++)
    {
      const size_t c = (*i).second;
      pond_section[c] = p;
    }

  pond_average = p; 
}

void
SurfaceStandardModel::set_svat_temperature (double T_surface /* dg C */)
{ 
  if (pond_average < 1e-5)
    T = T_surface;
}
  

void 
SurfaceStandardModel::initialize (const Geometry& geo)
{ 
  const std::vector<size_t>& top_edges = geo.cell_edges (Geometry::cell_above);
  const size_t top_edges_size = top_edges.size ();

  for (size_t i = 0; i < top_edges_size; i++)
    {
      const size_t edge = top_edges[i];
      pond_edge[edge] = i;
      if (pond_section.size () <= i)
        pond_section.push_back (0.0);
    }
  update_pond_average (geo);
}


SurfaceStandardModel::SurfaceStandardModel (double temperature_change_rate,
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

SurfaceStandardModel::~SurfaceStandardModel ()
{ }
