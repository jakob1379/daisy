#include "block_model.h"
#include "check.h"
#include "librarian.h"

#include "retention.h"
#include "models/retention/retention_exponential_model.h"

struct RetentionExponentialComponent : Retention, RetentionExponentialModel
{
  RetentionExponentialComponent (const BlockModel &al)
    : Retention (),
      RetentionExponentialModel (al.number ("k"),
                                 al.number ("Theta_res", -1.0),
                                 al.number ("h_min", 1.0),
                                 al.number ("Theta_sat", -1.0))
  { }
  ~RetentionExponentialComponent ()
  { }
};


static struct RetentionExponentialSyntax : DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new RetentionExponentialComponent (al); }
  RetentionExponentialSyntax ()
    : DeclareModel (Retention::component, "exp", "\
Retention curve of Mulch used in Exponential.\n\
Theta (h) = exp (k h) (Theta_sat - Theta_res) + Theta_res\n\
h (Theta) = ln ((Theta - Theta_res) / (Theta_sat - Theta_res)) / k")
  { }
  void load_frame (Frame& frame) const
  {
    frame.declare ("k", "cm^-1", Check::non_negative (),
		   Attribute::Const, "\
Theta (h) = exp (k h) (Theta_sat - Theta_res) + Theta_res");
    frame.declare_fraction ("Theta_res", Attribute::OptionalConst, "\
Residual water.");
    frame.declare ("h_min", "cm", Attribute::OptionalConst, "\
Pressure below residual water.");
    frame.declare_fraction ("Theta_sat", Attribute::OptionalConst, "\
Residual water.");
  }
} RetentionExponential_syntax;
