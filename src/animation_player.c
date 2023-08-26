#include "includes/animation_player.h"
#include "includes/package.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

TINY_BURGER static uint32_t _currentIndex = -1;
TINY_BURGER static const Animation_t *_currentAnimation = NULL;

TINY_BURGER static float _frameCount = 0.0f;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER static bool
    __exists_id(const AnimationPlayer_t *const ap, uint32_t id);

    TINY_BURGER static void __set_current_animnation(const AnimationPlayer_t *const ap, uint32_t id);

    TINY_BURGER static void __update_current_animnation(void);

    TINY_BURGER static void __draw_current_animnation(Vector2 position);

#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER AnimationPlayer_t *create_animation_player(uint32_t size)
{
    AnimationPlayer_t *ap = (AnimationPlayer_t *)MemAlloc(sizeof(AnimationPlayer_t));
    if (ap == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create AnimationPlayer_t pointer.");
        return NULL;
    }

    ap->animation = (Animation_t *)MemAlloc(sizeof(Animation_t) * size);
    if (ap->animation == NULL)
    {
        MemFree(ap);
        ap = NULL;
        TraceLog(LOG_DEBUG, "Error to create Animation_t pointer.");
        return ap;
    }

    ap->maxSize = size;
    ap->currentSize = 0;
    return ap;
}

TINY_BURGER void add_frames_animation_player(AnimationPlayer_t *const ap, uint32_t id, Rectangle *frames, uint32_t size)
{
    if (ap->currentSize < ap->maxSize)
    {
        uint32_t index = ap->currentSize;
        ap->animation[index] = (Animation_t){
            id,
            size,
            NULL};

        ap->animation->frames = (Rectangle *)MemAlloc(sizeof(Rectangle) * size);
        // TODO: verify value is NULL
        for (size_t i = 0; i < size; ++i)
            ap->animation->frames[i] = frames[i];
        ap->currentSize += 1;
    }
}

TINY_BURGER void update_animation_player(AnimationPlayer_t *const ap)
{
    if (_frameCount > TINY_BURGER_FPS / 10)
    {
        __update_current_animnation();
        _frameCount = 0.0f;
    }
    else
    {
        _frameCount += 0.5;
    }
}

TINY_BURGER void draw_animation_player(const AnimationPlayer_t *const ap, Vector2 position)
{
    __draw_current_animnation(position);
}

TINY_BURGER void set_animation_player(AnimationPlayer_t *const ap, uint32_t id)
{
    if (__exists_id(ap, id))
    {
        __set_current_animnation(ap, id);
    }
}

TINY_BURGER void destroy_animation_player(AnimationPlayer_t **ptr)
{
    if ((*ptr) != NULL)
    {
        for (uint32_t i = 0; i < (*ptr)->currentSize; ++i)
        {
            uint32_t id = (*ptr)->animation[i].id;
            MemFree((*ptr)->animation[i].frames);
            (*ptr)->animation[i].frames = NULL;
            TraceLog(LOG_DEBUG, TextFormat("[%d] Frame pointer destroyed successfully.", id));
        }
        MemFree((*ptr)->animation);
        (*ptr)->animation = NULL;
        TraceLog(LOG_DEBUG, "Animation_t pointer destroyed successfully.");

        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "AnimationPlayer_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------

TINY_BURGER static bool __exists_id(const AnimationPlayer_t *const ap, uint32_t id)
{
    bool value = false;
    for (size_t i = 0; i < ap->currentSize; ++i)
    {
        if (ap->animation[i].id == id)
        {
            value = true;
            break;
        }
    }

    return value;
}

TINY_BURGER static void __set_current_animnation(const AnimationPlayer_t *const ap, uint32_t id)
{
    for (size_t i = 0; i < ap->currentSize; ++i)
    {
        if (ap->animation[i].id == id)
        {
            _currentAnimation = &ap->animation[i];
            _currentIndex = 0;
        }
    }
}

TINY_BURGER static void __update_current_animnation(void)
{
    if (_currentIndex >= (_currentAnimation->size - 1))
    {
        _currentIndex = 0;
    }
    else
    {
        _currentIndex += 1;
    }
}

TINY_BURGER static void __draw_current_animnation(Vector2 position)
{
    DrawTextureRec(
        globalPackage->textures[TB_TEXTURE_TYPE_TILE],
        _currentAnimation->frames[_currentIndex],
        (Vector2){
            position.x * TINY_BURGER_TILE,
            position.y * TINY_BURGER_TILE},
        RAYWHITE);
}