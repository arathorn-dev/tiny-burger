#ifndef __TINY_BURGER_UTILS_H__
#define __TINY_BURGER_UTILS_H__

#include "config.h"

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER int32_t *load_draw_map(const char *, size_t, size_t);
    TINY_BURGER void unload_draw_map(int32_t **);

    TINY_BURGER int32_t *load_path_map(const int32_t *const, size_t, size_t);
    TINY_BURGER void unload_path_map(int32_t **);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_UTILS_H__