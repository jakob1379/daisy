#ifndef RETENTION_EXPONENTIAL_MODEL_H
#define RETENTION_EXPONENTIAL_MODEL_H

#include "interfaces/retention_interface.h"

struct RetentionExponentialModel : public virtual IRetention
{
  double k; 			// [cm^-1]
  double Theta_res;		// []
  double h_min;			// [cm]
  double Theta_sat;		// []
  
  double h (const double Theta) const; // []->[cm]
  double Theta (const double h); // [cm]->[]
  void initialize (const double Theta_res_,
                   const double h_res_,
		   const double Theta_sat_,
                   Treelog& msg);
  RetentionExponentialModel (double k,
                             double Theta_res,
                             double h_min,
                             double Theta_sat);
  ~RetentionExponentialModel ();
};

#endif
