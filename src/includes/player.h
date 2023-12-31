#ifndef __TINY_BURGER_PLAYER_H__
#define __TINY_BURGER_PLAYER_H__

#include "config.h"
#include "animation_player.h"
#include "gun.h"

TINY_BURGER typedef enum {
    PLAYER_ANIMATION_IDLE = 10,
    PLAYER_ANIMATION_RUN,
    PLAYER_ANIMATION_IDLE_STAIR,
    PLAYER_ANIMATION_IDLE_STAIR_MOV,
    PLAYER_ANIMATION_RUN_STAIR,
    PLAYER_ANIMATION_CELEBRATE,
    PLAYER_ANIMATION_LOSE,
    PLAYER_ANIMATION_SHOOT,
    PLAYER_ANIMATION_FALLING,
} PlayerAnimation_u;

TINY_BURGER typedef struct
{
    Vector2 position;
    Rectangle collisionShape;
    AnimationPlayer_t *ap;
    Gun_t *gun;

    bool interpolation;
    bool falling;
} Player_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Player_t *create_player(Vector2 position);
    TINY_BURGER void update_player(Player_t *const player, const int32_t *const vector);
    TINY_BURGER void update_player_without_movement(Player_t *const player, uint32_t animation);
    TINY_BURGER void draw_player(const Player_t *const player);
    TINY_BURGER Rectangle get_collision_shape_player(const Player_t *const player);
    TINY_BURGER void set_falling_player(Player_t *const player, bool value);
    TINY_BURGER bool get_reset_level_player(const Player_t *const player);
    TINY_BURGER void destroy_player(Player_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_PLAYER_H__
