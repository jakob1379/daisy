#ifndef RETENTION_INTERFACE_H
#define RETENTION_INTERFACE_H

#include "symbol.h"

class Treelog;

struct RetentionInterface
{
  // Simulation.
  virtual double h (double Theta) const = 0;

  // Create and Destroy.
  virtual void initialize (const double Theta_res,
                           const double h_res,
			   const double Theta_sat,
                           Treelog& msg) = 0;
  virtual ~RetentionInterface ()
  { }
};

#endif // RETENTION_INTERFACE_H
