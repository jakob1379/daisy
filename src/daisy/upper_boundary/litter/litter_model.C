#include "log.h"

#include "models/litter/litter_model.h"

void LitterModel::output (Log& log) const
{
  output_value (cover (), "cover", log);
}

double LitterModel::intercept () const
{ return 1.0; }

bool LitterModel::diffuse () const
{ return false; }

double LitterModel::water_protected () const
{ return 0.0; }

double LitterModel::potential_exfiltration () const
{ return 0.0; }

double LitterModel::decompose_factor () const
{ return 1.0; }
  
LitterModel::LitterModel()
{ }

LitterModel::~LitterModel ()
{ }
