// surface_simple_model.h -- Simple surface models.
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

#ifndef SURFACE_SIMPLE_MODEL_H
#define SURFACE_SIMPLE_MODEL_H

#include "interfaces/surface_interface.h"

struct SurfaceSimpleModel : public virtual SurfaceInterface {
  // Content.
  double T; 			// [dg C]
  
  // Communication with soil water.
  void accept_top (double amount, const Geometry&, size_t edge, double dt, Treelog&);

  // Column.
  double runoff_rate () const; // [h^-1]
  double mixing_resistance () const; // [h/mm]
  double mixing_depth () const; // [cm]

  // Manager.
  void set_detention_capacity (double);

  // Simulation.
  void update_pond_average (const Geometry&);
  void tick (const Time&, double dt /* [h] */,
             double PotSoilEvaporationWet, 
             double PotSoilEvaporationDry, 
             double flux_in /* [mm/h] */,
             double temp /* [dg C] */, const Geometry& geo,
             const Soil&, const SoilWater&,
             double soil_T /* [dg C] */, Treelog&);

  // Communication with bioclimate.
  double ponding_average () const; // [mm]
  double ponding_max () const;     // [mm]
  double temperature () const;     // [dg C]
  double EpFactor () const;        // []
  double albedo (const Geometry&, const Soil&, const SoilWater&) const;
  double exfiltration (double dt) const; // [mm/h]
  double evap_soil_surface () const; // [mm/h]
  double evap_pond (double dt, Treelog&) const; // [mm/h]
  void put_ponding (double pond);	// [mm]
  void set_svat_temperature (double temp /* dg C */);
  
  // Create.
  void initialize (const Geometry&);
  SurfaceSimpleModel ()
  { }
  ~SurfaceSimpleModel ()
  { }
};

struct SurfaceConstFluxModel : public SurfaceSimpleModel
{
  // Content.
  const double flux;

  top_t top_type (const Geometry&, size_t edge) const;
  double q_top (const Geometry&, size_t edge, const double dt) const; // [cm/h]
  double h_top (const Geometry& geo, size_t edge) const; // [cm]

  // Create.
  SurfaceConstFluxModel (double flux)
    : SurfaceSimpleModel (),
      flux (flux) 
  { }
  ~SurfaceConstFluxModel ()
  { }
};


struct SurfaceConstPressureModel : public SurfaceSimpleModel
{
  // Content.
  const double pressure;

  top_t top_type (const Geometry&, size_t edge) const;
  double q_top (const Geometry& geo, size_t edge, const double dt) const; // [cm/h]
  double h_top (const Geometry&, size_t edge) const; // [cm]

  // Create.
  SurfaceConstPressureModel (double pressure)
    : SurfaceSimpleModel (),
      pressure (pressure)
  { }
  ~SurfaceConstPressureModel ()
  { }
};

#endif // SURFACE_SIMPLE_MODEL_H
