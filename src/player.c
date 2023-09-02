#include "includes/player.h"
#include "includes/package.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
TINY_BURGER typedef enum {
    PLAYER_ANIMATION_IDLE = 10,
    PLAYER_ANIMATION_RUN,
    PLAYER_ANIMATION_IDLE_STAIR,
    PLAYER_ANIMATION_IDLE_STAIR_MOV,
    PLAYER_ANIMATION_RUN_STAIR,
} PlayerAnimation_u;

TINY_BURGER static bool _flipH = true;

TINY_BURGER static float _interpolationValue = 0.0f;
TINY_BURGER static float _interpolationMaxValue = 0.8f;
TINY_BURGER static Vector2 _interpolationPosition = {0};

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER static void __movement_player(Player_t *const player, const int32_t *const vector);
    TINY_BURGER static void __horizontal_movement(Vector2 *const position, const int32_t *const vector, int32_t factor);
    TINY_BURGER static void __vertical_movement(Vector2 *const position, const int32_t *const vector, int32_t factor);

    TINY_BURGER static void __reset_interpolation(Player_t *const player);
    TINY_BURGER static void __linear_interpolation(Player_t *const player);

    TINY_BURGER static AnimationPlayer_t *__init_animation_player(void);

#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER Player_t *create_player(Vector2 position)
{
    Player_t *player = MemAlloc(sizeof(Player_t));
    if (player == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Player_t pointer.");
        return NULL;
    }

    player->ap = __init_animation_player();
    if (player->ap == NULL)
    {
        MemFree(player);
        player = NULL;
        return player;
    }

    player->position = position;
    player->collisionShape = (Rectangle){
        position.x * TINY_BURGER_TILE,
        position.y * TINY_BURGER_TILE,
        TINY_BURGER_TILE,
        TINY_BURGER_TILE};
    player->isInterpolation = false;
    TraceLog(LOG_DEBUG, "Player_t pointer created successfully.");
    return player;
}

TINY_BURGER void update_player(Player_t *const player, const int32_t *const vector)
{
    if (player->isInterpolation)
    {
        __linear_interpolation(player);
    }
    else
    {
        __movement_player(player, vector);
    }
    update_animation_player(player->ap);
}

TINY_BURGER void draw_player(const Player_t *const player)
{
    draw_animation_player(player->ap, player->position, _flipH);
}

TINY_BURGER void destroy_player(Player_t **ptr)
{
    if ((*ptr) != NULL)
    {
        __reset_interpolation((*ptr));
        destroy_animation_player(&((*ptr)->ap));
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "App_t pointer destroyed successfully.");
    }
}

TINY_BURGER Rectangle get_collision_shape_player(const Player_t *const player)
{
    return (Rectangle){
        player->position.x * TINY_BURGER_TILE,
        player->position.y * TINY_BURGER_TILE,
        TINY_BURGER_TILE,
        TINY_BURGER_TILE,
    };
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __movement_player(Player_t *const player, const int32_t *const vector)
{
    Vector2 position = player->position;
    int32_t i = player->position.y;
    int32_t j = player->position.x;
    PlayerAnimation_u animation = PLAYER_ANIMATION_IDLE;
    uint32_t currentTile = (i > -1) ? (vector[j + i * TINY_BURGER_MAP_WIDTH] - 1) : 0;

    if (IsKeyDown(KEY_UP))
    {
        __vertical_movement(&position, vector, -1);
    }
    else if (IsKeyDown(KEY_LEFT) && position.x > 0 && position.y < (TINY_BURGER_MAP_HEIGHT - 1))
    {
        __horizontal_movement(&position, vector, -1);
    }
    else if (IsKeyDown(KEY_DOWN) && position.y < (TINY_BURGER_MAP_HEIGHT - 1))
    {
        __vertical_movement(&position, vector, 1);
    }
    else if (IsKeyDown(KEY_RIGHT) && position.x < (TINY_BURGER_MAP_WIDTH - 1) && position.y < (TINY_BURGER_MAP_HEIGHT - 1))
    {
        __horizontal_movement(&position, vector, 1);
    }

    if (player->position.x != position.x || player->position.y != position.y)
    {
        if (player->position.y != position.y)
            animation = PLAYER_ANIMATION_RUN_STAIR;
        else
            animation = PLAYER_ANIMATION_RUN;
        player->isInterpolation = true;
        _interpolationPosition = position;
    }
    else
    {
        uint32_t downTile = (i > -1) ? (vector[j + (i + 1) * TINY_BURGER_MAP_WIDTH] - 1) : 0;
        if (downTile >= 0 && downTile <= 2 && currentTile >= 1 && currentTile <= 4)
            animation = PLAYER_ANIMATION_IDLE_STAIR;
        else if (downTile >= 3 && downTile <= 4 && currentTile >= 1 && currentTile <= 4)
            animation = PLAYER_ANIMATION_IDLE_STAIR_MOV;
    }
    set_animation_player(player->ap, animation);
}

TINY_BURGER static void __vertical_movement(Vector2 *const position, const int32_t *const vector, int32_t factor)
{
    int32_t nextIndex = position->x + ((position->y + factor) * TINY_BURGER_MAP_WIDTH);
    int32_t currentIndex = position->x + (position->y * TINY_BURGER_MAP_WIDTH);
    int32_t currentTile = vector[currentIndex] - 1;
    if (factor < 0)
    {
        if (nextIndex > -1 && currentTile >= 1 && currentTile <= 4)
            position->y += 1 * factor;
        else if (currentTile >= 1 && currentTile <= 2)
            position->y += 1 * factor;
    }
    else
    {
        int32_t nextTile = vector[nextIndex] - 1;
        if ((position->y >= 0) && (currentTile >= -1 && currentTile <= 4 && nextTile >= 1 && nextTile <= 4))
            position->y += 1 * factor;
        else if (nextTile >= 1 && nextTile <= 2)
            position->y += 1 * factor;
    }
}

TINY_BURGER static void __horizontal_movement(Vector2 *const position, const int32_t *const vector, int32_t factor)
{
    int32_t nextIndex = (position->x + 1 * factor) + ((position->y + 1) * TINY_BURGER_MAP_WIDTH);
    int32_t nextTile = vector[nextIndex] - 1;

    if (nextTile >= 0 && nextTile <= 2)
    {
        _flipH = factor > 0;
        position->x += 1 * factor;
    }
}

TINY_BURGER static void __reset_interpolation(Player_t *const player)
{
    _interpolationValue = 0.0f;
    _interpolationPosition = (Vector2){0};
    player->isInterpolation = false;
}

TINY_BURGER static void __linear_interpolation(Player_t *const player)
{
    if (_interpolationValue > _interpolationMaxValue)
    {
        player->position.x = _interpolationPosition.x;
        player->position.y = _interpolationPosition.y;
        __reset_interpolation(player);
    }
    else
    {
        player->position.x = player->position.x + (_interpolationPosition.x - player->position.x) * _interpolationValue;
        player->position.y = player->position.y + (_interpolationPosition.y - player->position.y) * _interpolationValue;
        _interpolationValue += 0.1f;
    }
}

TINY_BURGER static AnimationPlayer_t *__init_animation_player(void)
{
    AnimationPlayer_t *ap = create_animation_player(5);
    if (ap != NULL)
    {
        Rectangle idle[] = {
            (Rectangle){0, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        Rectangle run[] = {
            (Rectangle){2 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){3 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        Rectangle idleStair[] = {
            (Rectangle){4 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        Rectangle idleStairUp[] = {
            (Rectangle){5 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        Rectangle runUp[] = {
            (Rectangle){5 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){6 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        add_frames_animation_player(ap, PLAYER_ANIMATION_IDLE, idle, 2);
        add_frames_animation_player(ap, PLAYER_ANIMATION_RUN, run, 2);
        add_frames_animation_player(ap, PLAYER_ANIMATION_IDLE_STAIR, idleStair, 1);
        add_frames_animation_player(ap, PLAYER_ANIMATION_IDLE_STAIR_MOV, idleStairUp, 1);
        add_frames_animation_player(ap, PLAYER_ANIMATION_RUN_STAIR, runUp, 2);
        set_animation_player(ap, PLAYER_ANIMATION_IDLE_STAIR);
    }

    return ap;
}
