#ifndef __TINY_BURGER_GUN_H__
#define __TINY_BURGER_GUN_H__

#include "config.h"
#include "raylib.h"

TINY_BURGER typedef struct
{
    Vector2 position;

} Gun_t;

#if defined(__cpluplus)
extern "C"
{
#endif

    TINY_BURGER Gun_t *create_gun(Vector2 position);
    TINY_BURGER void update_gun(Gun_t *const gun, Vector2 position);
    TINY_BURGER void draw_gun(const Gun_t *const gun);
    TINY_BURGER void destroy_gun(Gun_t **ptr);

#if defined(__cpluplus)
}
#endif

#endif // __TINY_BURGER_GUN_H__
