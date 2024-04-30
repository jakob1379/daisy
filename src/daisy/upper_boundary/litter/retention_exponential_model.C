#include <sstream>

#include "mathlib.h"
#include "treelog.h"

#include "models/litter/retention_exponential_model.h"

double RetentionExponentialModel::h (const double Theta) const // []->[cm]
{
    
  if (Theta <= Theta_res)
    return h_min;
  if (Theta >= Theta_sat)
    return 0.0;

  return std::log ((Theta - Theta_res) / (Theta_sat - Theta_res)) / k;
}

double RetentionExponentialModel::Theta (const double h) // [cm]->[]
{ return std::exp (k * h) * (Theta_sat - Theta_res) + Theta_res;  }
  
void RetentionExponentialModel::initialize (const double Theta_res_,
                                            const double h_res_,
                                            const double Theta_sat_,
                                            Treelog& msg)
{
  if (Theta_res < 0.0)
    Theta_res = Theta_res_;
  if (h_min > 0.0)
    h_min = h_res_;
  if (Theta_sat < 0.0)
    Theta_sat = Theta_sat_;

  std::ostringstream tmp;
  tmp << "pF\th\tTheta";
  for (double pF = -1.0; pF < 10.5; pF += 1.0)
    {
      const double h1 = pF2h (pF);
      const double Theta1 = Theta (h1);
      const double h2 = h (Theta1);
      tmp << "\n" << pF << "\t" << h2 << "\t" << Theta1;
    }
  msg.debug (tmp.str ());
}

RetentionExponentialModel::RetentionExponentialModel (double k,
                                                      double Theta_res,
                                                      double h_min,
                                                      double Theta_sat)
  : k (k),
    Theta_res (Theta_res),
    h_min (h_min),
    Theta_sat (Theta_sat)
{ }

RetentionExponentialModel::~RetentionExponentialModel ()
{ }

