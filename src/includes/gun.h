#ifndef __TINY_BURGER_GUN_H__
#define __TINY_BURGER_GUN_H__

#include "config.h"
#include "raylib.h"
#include "bullet.h"

#define BULLET_AMOUNT 5

TINY_BURGER typedef struct
{
    Vector2 position;
    Bullet_t *bulletList[BULLET_AMOUNT];

} Gun_t;

#if defined(__cpluplus)
extern "C"
{
#endif

    TINY_BURGER Gun_t *create_gun(Vector2 position);
    TINY_BURGER void update_gun(Gun_t *const gun, bool isShooting, bool right, Vector2 position);
    TINY_BURGER void draw_gun(const Gun_t *const gun);
    TINY_BURGER void destroy_gun(Gun_t **ptr);

#if defined(__cpluplus)
}
#endif

#endif // __TINY_BURGER_GUN_H__
