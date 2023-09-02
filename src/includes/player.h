#ifndef __TINY_BURGER_PLAYER_H__
#define __TINY_BURGER_PLAYER_H__

#include "config.h"
#include "animation_player.h"

TINY_BURGER typedef struct
{
    Vector2 position;
    AnimationPlayer_t *ap;
    bool isInterpolation;
    Rectangle collisionShape;
} Player_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Player_t *create_player(Vector2 position);
    TINY_BURGER void update_player(Player_t *const player, const int32_t *const vector);
    TINY_BURGER void draw_player(const Player_t *const player);
    TINY_BURGER void destroy_player(Player_t **ptr);
    TINY_BURGER Rectangle get_collision_shape_player(const Player_t *const player);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_PLAYER_H__