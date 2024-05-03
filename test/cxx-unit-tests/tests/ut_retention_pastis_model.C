#include <random>

#include <gtest/gtest.h>

#include "treelog_text.h"
#include "models/litter/retention_pastis_model.h"

struct RetentionPASTISModelTest : public testing::Test
{
  TreelogString msg;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<> h_min_dis;
  std::uniform_real_distribution<> Theta_dis;
  std::uniform_real_distribution<> Theta_res_dis;
  std::uniform_real_distribution<> Theta_sat_dis;
  RetentionPASTISModel retention;
  
  RetentionPASTISModelTest ()
    : rd(),
      gen(rd()),
      h_min_dis(-4e+06, -4e+03),
      Theta_dis(0.35, 0.65),
      Theta_res_dis(0.0001, 0.35),
      Theta_sat_dis(0.65, 0.9999),
      retention ()
  { }
};

TEST_F (RetentionPASTISModelTest, h)
{
  double h_min = h_min_dis (gen);
  double Theta = Theta_dis (gen);
  double Theta_res = Theta_res_dis (gen);
  double Theta_sat = Theta_sat_dis (gen);
  retention.initialize(Theta_res, h_min, Theta_sat, msg);
 
  double h = - std::pow (-h_min, 1 - ((Theta - Theta_res) / (2.0/3.0 * Theta_sat - Theta_res)));
  EXPECT_GE(Theta, Theta_res);
  EXPECT_LE(Theta, Theta_sat);
  EXPECT_DOUBLE_EQ (h, retention.h (Theta));
}
