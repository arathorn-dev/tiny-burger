#include "includes/player.h"
#include "includes/package.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER static void __movement_player(Player_t *const player, const int32_t *const vector);
    TINY_BURGER static void __horizontal_movement(Vector2 *const position, const int32_t *const vector, int32_t factor);
    TINY_BURGER static void __vertical_movement(Vector2 *const position, const int32_t *const vector, int32_t factor);

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

    player->position = position;
    TraceLog(LOG_DEBUG, "Player_t pointer created successfully.");
    return player;
}
TINY_BURGER void update_player(Player_t *const player, const int32_t *const vector)
{
    __movement_player(player, vector);
}
TINY_BURGER void draw_player(const Player_t *const player)
{
    DrawRectangle(
        player->position.x * TINY_BURGER_TILE,
        player->position.y * TINY_BURGER_TILE,
        TINY_BURGER_TILE,
        TINY_BURGER_TILE,
        GetColor(TINY_BURGER_COLOR_3));
}
TINY_BURGER void destroy_player(Player_t **ptr)
{
    if ((*ptr) != NULL)
    {
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
    Vector2 position = player->position;
    if (IsKeyPressed(KEY_UP))
    {
        __vertical_movement(&position, vector, -1);
    }
    else if (IsKeyPressed(KEY_LEFT) && position.x > 0 && position.y < (TINY_BURGER_MAP_HEIGHT - 1))
    {
        __horizontal_movement(&position, vector, -1);
    }
    else if (IsKeyPressed(KEY_DOWN) && position.y < (TINY_BURGER_MAP_HEIGHT - 1))
    {
        __vertical_movement(&position, vector, 1);
    }
    else if (IsKeyPressed(KEY_RIGHT) && position.x < (TINY_BURGER_MAP_WIDTH - 1) && position.y < (TINY_BURGER_MAP_HEIGHT - 1))
    {
        __horizontal_movement(&position, vector, 1);
    }

    player->position = position;
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
        position->x += 1 * factor;
}
