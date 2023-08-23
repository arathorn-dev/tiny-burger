#ifndef __TINY_BURGER_UTILS_H__
#define __TINY_BURGER_UTILS_H__

#include "config.h"

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER int32_t *load_map(const char *, size_t, size_t);
    TINY_BURGER void unload_map(int32_t **);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_UTILS_H__