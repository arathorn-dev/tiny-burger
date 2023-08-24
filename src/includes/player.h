#ifndef __TINY_BURGER_PLAYER_H__
#define __TINY_BURGER_PLAYER_H__

#include "config.h"

TINY_BURGER typedef struct
{
    Vector2 position;
} Player_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Player_t *create_player(Vector2 position);
    TINY_BURGER void update_player(Player_t *const player);
    TINY_BURGER void draw_player(const Player_t *const player);
    TINY_BURGER void destroy_player(Player_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_PLAYER_H__