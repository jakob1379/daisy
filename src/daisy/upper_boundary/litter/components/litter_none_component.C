#include "block_model.h"
#include "librarian.h"

#include "litter.h"
#include "models/litter/litter_none_model.h"

struct LitterNoneComponent : Litter, LitterNoneModel
{
  void output (Log& log) const
  {
    LitterNoneModel::output (log);
  }
  
  LitterNoneComponent (const BlockModel& al)
    : Litter (al),
      LitterNoneModel()
  {}
};

static struct LitterNoneSyntax : DeclareModel
{
  Model* make (const BlockModel& al) const
  { return new LitterNoneComponent (al); }
  LitterNoneSyntax ()
    : DeclareModel (Litter::component, "none", "\
The effect of surface residuals is ignored by the model.")
  { }
  void load_frame (Frame&) const
  { }
} LitterNone_syntax;
