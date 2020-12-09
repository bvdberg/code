#ifndef CONFIG_H
#define CONFIG_H

// un-comment ONE of these
//#include "config_a.h"
#include "config_b.h"
//#include "config_c.h"


// --- SUB-CONFIGS ---

#ifdef FEATURE_2
#define FEATURE_X
#endif

#endif

