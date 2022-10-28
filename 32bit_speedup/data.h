#ifndef DATA_H
#define DATA_H

#include <stdint.h>
#ifdef   __cplusplus
extern "C"
{
#endif

void data_init(uint64_t max);

void data_free(void);

void data_add(void* d);

#ifdef   __cplusplus
}
#endif

#endif

