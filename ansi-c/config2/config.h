#ifndef CONFIG_H
#define CONFIG_H

#if _VARIANT == 1

#define FEATURE_1
//#define FEATURE_2
//#define FEATURE_3

#elif _VARIANT == 2

//#define FEATURE_1
#define FEATURE_2
//#define FEATURE_3

#else

#warning "unknown variant"

#endif

#endif

