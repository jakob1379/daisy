#include "librarian.h"
#include "retention.h"

static struct RetentionInit : public DeclareComponent 
{
  RetentionInit ()
    : DeclareComponent (Retention::component, "\
Specify a retention or a halftime.")
  { }
} Retention_init;
