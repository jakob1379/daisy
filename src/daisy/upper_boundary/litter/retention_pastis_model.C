#include "mathlib.h"
#include "models/retention/retention_pastis_model.h"

double RetentionPASTISModel::h (const double Theta) const
{
  if (Theta < Theta_res)
    return h_min;
  if (Theta > Theta_sat)
    return 0.0;

  return -std::pow (-h_min,
                    1.0-((Theta - Theta_res)
                         / (2.0 * Theta_sat / 3.0 - Theta_res)));
}

void RetentionPASTISModel::initialize (const double Theta_res_,
                                       const double h_res_,
                                       const double Theta_sat_,
                                       Treelog&)
{
  Theta_res = Theta_res_;
  h_min = h_res_;
  Theta_sat = Theta_sat_;
}

RetentionPASTISModel::RetentionPASTISModel ()
{ }

RetentionPASTISModel::~RetentionPASTISModel ()
{ }
