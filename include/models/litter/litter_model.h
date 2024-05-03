#ifndef LITTER_MODEL_H
#define LITTER_MODEL_H

#include "interfaces/litter_interface.h"

struct LitterModel : public virtual ILitter
{
  void output (Log& log) const;
  double intercept () const;
  bool diffuse () const;
  double water_protected () const;
  double potential_exfiltration () const;
  double decompose_factor () const;
  
  // Create and Destroy.
  LitterModel();
  ~LitterModel ();
};
#endif // LITTER_MODEL_H
