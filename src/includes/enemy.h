#ifndef __TINY_BURGER_ENEMY_H__
#define __TINY_BURGER_ENEMY_H__

#include "config.h"
#include "animation_player.h"

TINY_BURGER typedef enum {
    TB_ENEMY_TYPE_HOT_DOG = 0,
    TB_ENEMY_TYPE_EGG,
    TB_ENEMY_TYPE_PICKLE,
} EnemyType_u;

TINY_BURGER typedef struct
{
    Vector2 position;
    EnemyType_u type;
    AnimationPlayer_t *ap;
} Enemy_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Enemy_t *create_enemy(EnemyType_u type, Vector2 position);
    TINY_BURGER void update_enemy(Enemy_t *const enemy);
    TINY_BURGER void draw_enemy(const Enemy_t *const enemy);
    TINY_BURGER void destroy_enemy(Enemy_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_ENEMY_H__