#ifndef __TINY_BURGER_ENEMY_H__
#define __TINY_BURGER_ENEMY_H__

#include "config.h"
#include "animation_player.h"
#include "player.h"

TINY_BURGER typedef enum {
    TB_ENEMY_TYPE_PICKLE = 0,
    TB_ENEMY_TYPE_EGG,
    TB_ENEMY_TYPE_HOT_DOG
} EnemyType_u;

TINY_BURGER typedef enum {
    TB_ENEMY_STATE_FOLLOWING,
    TB_ENEMY_STATE_DIZZY,
    TB_ENEMY_STATE_FALLING,
    TB_ENEMY_STATE_RESTATING,
} EnemyState_t;

TINY_BURGER typedef struct
{
    Vector2 position;
    Vector2 spawnPosition;
    Vector2 playerPosition;
    Vector2 vectorList[TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT];
    Vector2 interpolationPosition;
    EnemyType_u type;
    AnimationPlayer_t *ap;
    uint32_t vectorListIndex;
    Rectangle collisionShape;
    EnemyState_t state;

    bool interpolation;
    bool flipH;

    float interpolationValue;
    uint32_t internalFPSCounter;

} Enemy_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Enemy_t *create_enemy(EnemyType_u type, Vector2 position);
    TINY_BURGER void update_enemy(Enemy_t *const enemy, const int32_t *const map, Player_t *const player);
    TINY_BURGER void draw_enemy(const Enemy_t *const enemy);
    TINY_BURGER void destroy_enemy(Enemy_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_ENEMY_H__