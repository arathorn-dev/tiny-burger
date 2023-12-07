#include "includes/player.h"
#include "includes/config.h"
#include "includes/gun.h"
#include "includes/package.h"
#include "includes/gui.h"
#include "includes/raylib.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;
extern GuiData_t *globalGuiData;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
TINY_BURGER static bool _isFlipH = true;
TINY_BURGER static bool _isShooting = false;
TINY_BURGER static bool _isResetLevel = false;

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
    TINY_BURGER static void __update_collision_shape_value(Player_t *const player);
    TINY_BURGER static void __reset_interpolation(Player_t *const player);
    TINY_BURGER static void __linear_interpolation(Player_t *const player);
    TINY_BURGER static AnimationPlayer_t *__init_animation_player(void);

    TINY_BURGER static void __falling_movement(Player_t *const player);
    TINY_BURGER static void __keyboard_movement(Player_t *const player, const int32_t *const vector);
    TINY_BURGER static void __reset_static_values(void);

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

    player->gun = create_gun(position);
    if (player->gun == NULL)
    {
        MemFree(player);
        player = NULL;
        return player;
    }

    player->ap = __init_animation_player();
    if (player->ap == NULL)
    {
        destroy_gun(&(player->gun));
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
    player->interpolation = false;
    player->falling = false;

    __reset_static_values();
    TraceLog(LOG_DEBUG, "Player_t pointer created successfully.");
    return player;
}

TINY_BURGER void update_player(Player_t *const player, const int32_t *const vector)
{

    if (player->interpolation)
        __linear_interpolation(player);
    else
        __movement_player(player, vector);

    __update_collision_shape_value(player);
    update_gun(player->gun, _isShooting, _isFlipH, player->position);
    update_animation_player(player->ap);
}

TINY_BURGER void update_player_without_movement(Player_t *const player, uint32_t animation)
{
    set_animation_player(player->ap, animation);
    update_animation_player(player->ap);
    update_gun(player->gun, _isShooting, _isFlipH, player->position);
    __update_collision_shape_value(player);
}

TINY_BURGER void draw_player(const Player_t *const player)
{
    draw_animation_player(player->ap, player->position, _isFlipH);
    draw_gun(player->gun);
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

TINY_BURGER void set_falling_player(Player_t *const player, bool value)
{
    player->falling = value;
}

TINY_BURGER bool get_reset_level_player(const Player_t *const player)
{
    return _isResetLevel;
}

TINY_BURGER void destroy_player(Player_t **ptr)
{
    if ((*ptr) != NULL)
    {
        __reset_interpolation((*ptr));
        destroy_animation_player(&((*ptr)->ap));
        destroy_gun(&((*ptr)->gun));
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "App_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __movement_player(Player_t *const player, const int32_t *const vector)
{
    if (player->falling)
        __falling_movement(player);
    else
        __keyboard_movement(player, vector);
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
        _isFlipH = factor > 0;
        position->x += 1 * factor;
    }
}

TINY_BURGER static void __reset_interpolation(Player_t *const player)
{
    _interpolationValue = 0.0f;
    _interpolationPosition = (Vector2){0};
    player->interpolation = false;
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
    AnimationPlayer_t *ap = create_animation_player(TINY_BURGER_PLAYER_ANIMATION_LENGTH);
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

        Rectangle celebrate[] = {
            (Rectangle){7 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){8 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){9 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        Rectangle shoot[] = {
            (Rectangle){10 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){11 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        Rectangle falling[] = {
            (Rectangle){12 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){13 * TINY_BURGER_TILE, 4 * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        add_frames_animation_player(ap, PLAYER_ANIMATION_IDLE, idle, 2);
        add_frames_animation_player(ap, PLAYER_ANIMATION_RUN, run, 2);
        add_frames_animation_player(ap, PLAYER_ANIMATION_IDLE_STAIR, idleStair, 1);
        add_frames_animation_player(ap, PLAYER_ANIMATION_IDLE_STAIR_MOV, idleStairUp, 1);
        add_frames_animation_player(ap, PLAYER_ANIMATION_RUN_STAIR, runUp, 2);
        add_frames_animation_player(ap, PLAYER_ANIMATION_CELEBRATE, celebrate, 3);
        add_frames_animation_player(ap, PLAYER_ANIMATION_SHOOT, shoot, 2);
        add_frames_animation_player(ap, PLAYER_ANIMATION_FALLING, falling, 2);
        set_animation_player(ap, PLAYER_ANIMATION_IDLE_STAIR);
    }

    return ap;
}

TINY_BURGER static void __update_collision_shape_value(Player_t *const player)
{
    player->collisionShape.x = player->position.x * TINY_BURGER_TILE;
    player->collisionShape.y = player->position.y * TINY_BURGER_TILE;
}

TINY_BURGER static void __keyboard_movement(Player_t *const player, const int32_t *const vector)
{
    Vector2 position = player->position;
    int32_t i = player->position.y;
    int32_t j = player->position.x;
    PlayerAnimation_u animation = PLAYER_ANIMATION_IDLE;
    uint32_t currentTile = (i > -1) ? (vector[j + i * TINY_BURGER_MAP_WIDTH] - 1) : 0;
    _isShooting = false;

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
    else if (globalGuiData->bulletAmount > 0 && IsKeyDown(KEY_SPACE))
    {
        _isShooting =
            player->ap->currentAnimation->id != PLAYER_ANIMATION_RUN_STAIR &&
            player->ap->currentAnimation->id != PLAYER_ANIMATION_IDLE_STAIR_MOV;
        if (_isShooting)
        {
            animation = PLAYER_ANIMATION_SHOOT;
        }
    }

    if (!_isShooting && (player->position.x != position.x || player->position.y != position.y))
    {
        if (player->position.y != position.y)
            animation = PLAYER_ANIMATION_RUN_STAIR;
        else
            animation = PLAYER_ANIMATION_RUN;
        player->interpolation = true;
        _interpolationPosition = position;
    }
    else if (!_isShooting)
    {
        uint32_t downTile = (i > -1) ? (vector[j + (i + 1) * TINY_BURGER_MAP_WIDTH] - 1) : 0;
        if (downTile >= 0 && downTile <= 2 && currentTile >= 1 && currentTile <= 4)
            animation = PLAYER_ANIMATION_IDLE_STAIR;
        else if (downTile >= 3 && downTile <= 4 && currentTile >= 1 && currentTile <= 4)
            animation = PLAYER_ANIMATION_IDLE_STAIR_MOV;
    }

    set_animation_player(player->ap, animation);
}

TINY_BURGER static void __falling_movement(Player_t *const player)
{
    Vector2 position = player->position;
    position.y += 1;
    _interpolationPosition = position;
    player->interpolation = true;
    set_animation_player(player->ap, PLAYER_ANIMATION_FALLING);

    if (player->position.y >= TINY_BURGER_MAP_HEIGHT)
    {
        _isResetLevel = true;
    }
}

TINY_BURGER static void __reset_static_values(void)
{
    _isFlipH = true;
    _isShooting = false;
    _isResetLevel = false;
}