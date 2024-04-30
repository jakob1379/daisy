#ifndef RETENTION_PASTIS_H
#define RETENTION_PASTIS_H

#include "interfaces/retention_interface.h"

struct RetentionPASTISModel : public virtual RetentionInterface
{
  double Theta_res;
  double h_min;
  double Theta_sat;
  
  double h (const double Theta) const; // []->[cm]
  void initialize (const double Theta_res_,
                   const double h_res_,
		   const double Theta_sat_,
                   Treelog&);
  RetentionPASTISModel ();
  ~RetentionPASTISModel ();
};

#endif
