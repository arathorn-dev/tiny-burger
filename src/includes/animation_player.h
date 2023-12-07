#ifndef __TINY_BURGER_ANIMATION_H__
#define __TINY_BURGER_ANIMATION_H__

#include "config.h"

TINY_BURGER typedef enum {
    ENEMY_ANIMATION_IDLE = 10,
    ENEMY_ANIMATION_RUN,
    ENEMY_ANIMATION_IDLE_STAIR,
    ENEMY_ANIMATION_IDLE_STAIR_MOV,
    ENEMY_ANIMATION_RUN_STAIR,
    ENEMY_ANIMATION_DIZZY,
    ENEMY_ANIMATION_CELEBRATE,
    ENEMY_ANIMATION_CELEBRATE_STAIR,
    ENEMY_ANIMATION_FALLING,
} EnemyAnimation_u;

TINY_BURGER typedef struct
{
    uint32_t id;
    uint32_t size;
    Rectangle *frames;

} Animation_t;

TINY_BURGER typedef struct
{
    Animation_t *animation;
    uint32_t maxSize;
    uint32_t currentSize;

    uint32_t currentIndex;
    uint32_t currentId;
    Animation_t *currentAnimation;
    float frameCount;
    float frameValue;

} AnimationPlayer_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER AnimationPlayer_t *create_animation_player(uint32_t size);
    TINY_BURGER void update_animation_player(AnimationPlayer_t *const ap);
    TINY_BURGER void set_animation_player(AnimationPlayer_t *const ap, uint32_t id);
    TINY_BURGER void set_frame_add_animation_player(AnimationPlayer_t *const ap, float value);
    TINY_BURGER void draw_animation_player(const AnimationPlayer_t *const ap, Vector2 position, bool flipH);
    TINY_BURGER void add_frames_animation_player(AnimationPlayer_t *const ap, uint32_t id, Rectangle *frames, uint32_t size);
    TINY_BURGER void destroy_animation_player(AnimationPlayer_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_ANIMATION_H__