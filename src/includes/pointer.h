#ifndef __TINY_BURGER_POINTER_H__
#define __TINY_BURGER_POINTER_H__

#include "config.h"

TINY_BURGER typedef struct
{
    Vector2 position;
    Vector2 dimension;
    Rectangle base;
} Pointer_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Pointer_t *create_pointer(Vector2 position);
    TINY_BURGER void update_pointer(Pointer_t *const pointer, Vector2 position);
    TINY_BURGER void draw_pointer(const Pointer_t *const pointer);
    TINY_BURGER void destroy_pointer(Pointer_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_POINTER_H__