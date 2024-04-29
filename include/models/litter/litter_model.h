#ifndef LITTER_MODEL_H
#define LITTER_MODEL_H

#include "interfaces/litter_interface.h"

struct LitterModel : public virtual LitterInterface
{
  double intercept () const
  { return 1.0; }
  bool diffuse () const
  { return false; }
  double water_protected () const
  { return 0.0; }
  double potential_exfiltration () const
  { return 0.0; }
  double decompose_factor () const
  { return 1.0; }
  
  // Create and Destroy.
  LitterModel()
  { }
  ~LitterModel ()
  { }
};
#endif // LITTER_MODEL_H
