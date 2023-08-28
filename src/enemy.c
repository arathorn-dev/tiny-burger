#include "includes/enemy.h"
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
    TraceLog(LOG_DEBUG, "Enemy_t pointer created successfully.");
    return enemy;
}
TINY_BURGER void update_enemy(Enemy_t *const enemy)
{
    // TODO
}
TINY_BURGER void draw_enemy(const Enemy_t *const enemy)
{
    DrawRectangle(
        enemy->position.x * TINY_BURGER_TILE,
        enemy->position.y * TINY_BURGER_TILE,
        TINY_BURGER_TILE,
        TINY_BURGER_TILE,
        GetColor(TINY_BURGER_COLOR_3));
}
TINY_BURGER void destroy_enemy(Enemy_t **ptr)
{
    if ((*ptr) != NULL)
    {
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Enemy_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
