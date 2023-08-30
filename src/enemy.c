#include "includes/enemy.h"
#include "includes/package.h"
#include "includes/pathfinding.h"

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
    TINY_BURGER static bool __equals_position(Vector2 a, Vector2 b);
    TINY_BURGER static void __init_vector_list(Enemy_t *enemy);
    TINY_BURGER static void __destroy_vector_list(Enemy_t *enemy);
#if defined(__cplusplus)
}
#endif
//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER Enemy_t *create_enemy(EnemyType_u type, Vector2 position)
{
    Enemy_t *enemy = (Enemy_t *)MemAlloc(sizeof(Enemy_t));
    if (enemy == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Enemy_t pointer.");
        return NULL;
    }

    enemy->position = position;
    enemy->type = type;
    enemy->ap = NULL;

    enemy->playerPosition = (Vector2){0};
    __init_vector_list(enemy);

    TraceLog(LOG_DEBUG, "Enemy_t pointer created successfully.");
    return enemy;
}
TINY_BURGER void update_enemy(Enemy_t *const enemy, const int32_t *const map, const Player_t *const player)
{
    if (!player->isInterpolation && !__equals_position(enemy->playerPosition, player->position))
    {
        __destroy_vector_list(enemy);
        Vector2 enemyPosition = (Vector2){0};
        enemyPosition.x = enemy->position.x;
        enemyPosition.y = enemy->position.y + 1;

        Vector2 playerPosition = (Vector2){0};
        playerPosition.x = player->position.x;
        playerPosition.y = player->position.y + 1;

        get_path(enemy->vectorList, map, enemyPosition, playerPosition);
        // enemy->vectorList = get_path(map, playerPosition, enemyPosition);
        enemy->playerPosition.x = player->position.x;
        enemy->playerPosition.y = player->position.y;
    }
}
TINY_BURGER void draw_enemy(const Enemy_t *const enemy)
{
    DrawRectangle(
        enemy->position.x * TINY_BURGER_TILE,
        enemy->position.y * TINY_BURGER_TILE,
        TINY_BURGER_TILE,
        TINY_BURGER_TILE,
        GetColor(TINY_BURGER_COLOR_3));

    for (uint32_t i = 0; enemy->vectorList[i].x != -100; ++i)
    {
        DrawRectangleLines(
            enemy->vectorList[i].x * TINY_BURGER_TILE,
            (enemy->vectorList[i].y - 1) * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            GetColor(TINY_BURGER_COLOR_3));
    }
}
TINY_BURGER void destroy_enemy(Enemy_t **ptr)
{
    if ((*ptr) != NULL)
    {
        __destroy_vector_list((*ptr));
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Enemy_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static bool __equals_position(Vector2 a, Vector2 b)
{
    return a.x == b.x && a.y == b.y;
}

TINY_BURGER static void __init_vector_list(Enemy_t *enemy)
{
    uint32_t size = TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT;
    for (uint32_t i = 0; i < size; ++i)
        enemy->vectorList[i] = (Vector2){-100, -100};
}

TINY_BURGER static void __destroy_vector_list(Enemy_t *enemy)
{
    uint32_t i = 0;
    while (enemy->vectorList[i].x != -100)
    {
        enemy->vectorList[i] = (Vector2){-100, -100};
        ++i;
    }
}
