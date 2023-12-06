#ifndef __TINY_BURGER_BULLET_H__
#define __TINY_BURGER_BULLET_H__

#include "config.h"

TINY_BURGER typedef struct
{
    Rectangle rect;

    bool right;
    bool active;

    Color color;

} Bullet_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Bullet_t *create_bullet(Vector2 position, bool right);
    TINY_BURGER void update_bullet(Bullet_t *const bullet, Vector2 position);
    TINY_BURGER void draw_bullet(const Bullet_t *const bullet);
    TINY_BURGER void set_active_bullet(Bullet_t *bullet, bool right);
    TINY_BURGER void destroy_bullet(Bullet_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif //__TINY_BURGER_BULLET_H__