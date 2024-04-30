#include "block_model.h"
#include "librarian.h"
#include "litter.h"

static struct LitterInit : public DeclareComponent 
{
  LitterInit ()
    : DeclareComponent (Litter::component, "\
Litter, surface residuals, or mulch below canopy.")
  { }
  void load_frame (Frame& frame) const
  { 
    Model::load_model (frame); 
    frame.declare_fraction ("cover", Attribute::LogOnly, "\
Fraction of surface area covered by litter.");
  }
} Litter_init;
