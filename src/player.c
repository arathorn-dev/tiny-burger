#include "includes/player.h"
#include "includes/package.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

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
TINY_BURGER void update_player(Player_t *const player)
{
    // TODO
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