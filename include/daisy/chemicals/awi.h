// awi.h --- Air-Water Interface area.
// 
// Copyright 2025 KU.
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


#ifndef AWI_H
#define AWI_H

#include "object_model/model_derived.h"
#include "object_model/symbol.h"
#include <vector>

class Geometry;
class Soil;
class SoilWater;
class Treelog;
class BlockModel;

class AWI : public ModelDerived
{
  // Content.
public:
  static const char *const component;
  symbol library_id () const;

  // Calculated.
  std::vector<double> AWI_area;

  // Simulation.
protected:
  virtual double find_area (double Theta, double Theta_sat,
			    double d50 /* [cm] */) const = 0; // [cm^2/cm^3]

public:
  void output (Log&) const;
  void tick (const Geometry&, const Soil&, const SoilWater&);
  double area (size_t c) const // [cm^2/cm^3]
  { return (c < AWI_area.size ()) ? AWI_area[c] : 0.0; }

  // Create and Destroy.
protected:
  AWI (const BlockModel&);
public:
  ~AWI ();
};

#endif // AWI_H
