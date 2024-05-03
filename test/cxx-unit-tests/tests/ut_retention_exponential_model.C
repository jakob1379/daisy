#include <random>

#include <gtest/gtest.h>

#include "models/litter/retention_exponential_model.h"

struct RetentionExponentialModelTest : public testing::Test
{
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<> k_dis;
  std::uniform_real_distribution<> h_min_dis;
  std::uniform_real_distribution<> h_dis;
  std::uniform_real_distribution<> Theta_res_dis;
  std::uniform_real_distribution<> Theta_sat_dis;
  
  RetentionExponentialModelTest ()
    : rd(),
      gen(rd()),
      k_dis(.001, 0.01),
      h_min_dis(-4e+06, -4e+03),
      h_dis(-400, 0),
      Theta_res_dis(0.0001, 0.4),
      Theta_sat_dis(0.5, 0.9999)
  { }
};

TEST_F (RetentionExponentialModelTest, ThetaHRelation)
{
  double k = k_dis (gen);
  double Theta_res = Theta_res_dis (gen);
  double h_min = h_min_dis (gen);
  double Theta_sat = Theta_sat_dis (gen);
  double h = h_dis (gen);  
  RetentionExponentialModel retention (k, Theta_res, h_min, Theta_sat);
  
  double Theta = retention.Theta (h);
  EXPECT_GE(Theta, Theta_res);
  EXPECT_LE(Theta, Theta_sat);
  EXPECT_DOUBLE_EQ (std::exp (k*h) * (Theta_sat - Theta_res) + Theta_res, Theta);
  EXPECT_NEAR (h, retention.h(Theta), 1e-10);
}
