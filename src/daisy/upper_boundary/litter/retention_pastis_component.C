#include "block_model.h"
#include "librarian.h"

#include "retention.h"
#include "models/retention/retention_pastis_model.h"

struct RetentionPASTISComponent : Retention, RetentionPASTISModel
{
  RetentionPASTISComponent (const BlockModel &al)
    : Retention (),
      RetentionPASTISModel ()
  { }
  ~RetentionPASTISComponent ()
  { }
};

static struct RetentionPASTISSyntax : DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new RetentionPASTISComponent (al); }
  RetentionPASTISSyntax ()
    : DeclareModel (Retention::component, "PASTIS", "\
Retention curve of Mulch used in PASTIS.\n\
h (Theta) = -(-h_min)^(1-((Theta - Theta_res) / (2/3 Theta_sat - Theta_res)))")
  { }
  void load_frame (Frame&) const
  { }
} RetentionPASTIS_syntax;
