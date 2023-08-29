#ifndef __TINY_BURGER_UTILS_H__
#define __TINY_BURGER_UTILS_H__

#include "config.h"

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER int32_t *load_draw_map(const char *fileName, uint32_t width, uint32_t height);
    TINY_BURGER void unload_draw_map(int32_t **ptr);

    TINY_BURGER int32_t *load_path_map(const int32_t *const vectorDraw, uint32_t width, uint32_t height);
    TINY_BURGER void unload_path_map(int32_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_UTILS_H__