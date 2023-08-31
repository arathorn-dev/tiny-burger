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
TINY_BURGER static float _interpolationMaxValue = 0.8f;

TINY_BURGER typedef enum {
    ENEMY_ANIMATION_IDLE = 10,
    ENEMY_ANIMATION_RUN,
    ENEMY_ANIMATION_IDLE_STAIR,
    ENEMY_ANIMATION_IDLE_STAIR_MOV,
    ENEMY_ANIMATION_RUN_STAIR,
} EnemyAnimation_u;

#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER static bool __equals_position(Vector2 a, Vector2 b);
    TINY_BURGER static void __init_vector_list(Enemy_t *enemy);
    TINY_BURGER static void __destroy_vector_list(Enemy_t *enemy);
    TINY_BURGER static void __update_enemy_path(Enemy_t *const enemy, const int32_t *const map, const Player_t *const player);
    TINY_BURGER static void __movement(Enemy_t *const enemy);

    TINY_BURGER static void __linear_interpolation(Enemy_t *const enemy);

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
    // ----
    enemy->ap = create_animation_player(5);
    enemy->flipH = false;
    if (enemy->ap == NULL)
    {
        MemFree(enemy);
        enemy = NULL;
        return enemy;
    }
    uint32_t posY = 1;
    Rectangle idle[] = {
        (Rectangle){0, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        (Rectangle){TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
    };

    Rectangle run[] = {
        (Rectangle){2 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        (Rectangle){3 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
    };

    Rectangle idleStair[] = {
        (Rectangle){4 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
    };

    Rectangle idleStairUp[] = {
        (Rectangle){5 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
    };

    Rectangle runUp[] = {
        (Rectangle){5 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        (Rectangle){6 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
    };

    add_frames_animation_player(enemy->ap, ENEMY_ANIMATION_IDLE, idle, 2);
    add_frames_animation_player(enemy->ap, ENEMY_ANIMATION_RUN, run, 2);
    add_frames_animation_player(enemy->ap, ENEMY_ANIMATION_IDLE_STAIR, idleStair, 1);
    add_frames_animation_player(enemy->ap, ENEMY_ANIMATION_IDLE_STAIR_MOV, idleStairUp, 1);
    add_frames_animation_player(enemy->ap, ENEMY_ANIMATION_RUN_STAIR, runUp, 2);
    set_animation_player(enemy->ap, ENEMY_ANIMATION_IDLE_STAIR);
    // ----
    enemy->interpolationValue = 0;
    enemy->isInterpolation = false;

    enemy->playerPosition = (Vector2){0};
    __init_vector_list(enemy);

    TraceLog(LOG_DEBUG, "Enemy_t pointer created successfully.");
    return enemy;
}

TINY_BURGER void update_enemy(Enemy_t *const enemy, const int32_t *const map, const Player_t *const player)
{

    __update_enemy_path(enemy, map, player);
    __linear_interpolation(enemy);
    __movement(enemy);
    update_animation_player(enemy->ap);
}

TINY_BURGER void draw_enemy(const Enemy_t *const enemy)
{
    draw_animation_player(enemy->ap, enemy->position, enemy->flipH);
    // for (uint32_t i = 0; enemy->vectorList[i].x != -100; ++i)
    // {
    //     DrawRectangleLines(
    //         enemy->vectorList[i].x * TINY_BURGER_TILE,
    //         (enemy->vectorList[i].y - 1) * TINY_BURGER_TILE,
    //         TINY_BURGER_TILE,
    //         TINY_BURGER_TILE,
    //         GetColor(TINY_BURGER_COLOR_3));
    // }
}

TINY_BURGER void destroy_enemy(Enemy_t **ptr)
{
    if ((*ptr) != NULL)
    {
        destroy_animation_player(&((*ptr)->ap));
        __destroy_vector_list((*ptr));
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Enemy_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __update_enemy_path(Enemy_t *const enemy, const int32_t *const map, const Player_t *const player)
{
    if (!enemy->isInterpolation && !player->isInterpolation && !__equals_position(enemy->playerPosition, player->position))
    {
        __destroy_vector_list(enemy);
        Vector2 enemyPosition = (Vector2){0};
        enemyPosition.x = enemy->position.x;
        enemyPosition.y = enemy->position.y + 1;

        Vector2 playerPosition = (Vector2){0};
        playerPosition.x = player->position.x;
        playerPosition.y = player->position.y + 1;

        get_path(enemy->vectorList, map, playerPosition, enemyPosition);

        enemy->playerPosition.x = player->position.x;
        enemy->playerPosition.y = player->position.y;
        enemy->vectorListIndex = 1;
    }
}

TINY_BURGER static void __movement(Enemy_t *const enemy)
{
    if (enemy->isInterpolation)
    {
        EnemyAnimation_u animation = ENEMY_ANIMATION_RUN;
        uint32_t index = enemy->vectorListIndex;
        enemy->flipH = enemy->position.x < enemy->vectorList[index].x;
        if (enemy->position.y != (enemy->vectorList[index].y - 1))
            animation = ENEMY_ANIMATION_RUN_STAIR;

        set_animation_player(enemy->ap, animation);
    }
}

TINY_BURGER static bool __equals_position(Vector2 a, Vector2 b)
{
    return a.x == b.x && a.y == b.y;
}

TINY_BURGER static void __init_vector_list(Enemy_t *enemy)
{
    uint32_t size = TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT;
    for (uint32_t i = 0; i < size; ++i)
        enemy->vectorList[i] = (Vector2){-100, -100};

    enemy->vectorListIndex = 0;
}

TINY_BURGER static void __destroy_vector_list(Enemy_t *enemy)
{
    uint32_t i = 0;
    while (enemy->vectorList[i].x != -100)
    {
        enemy->vectorList[i] = (Vector2){-100, -100};
        ++i;
    }
    enemy->vectorListIndex = 0;
}

TINY_BURGER static void __linear_interpolation(Enemy_t *const enemy)
{
    Vector2 position = (Vector2){
        enemy->vectorList[enemy->vectorListIndex].x,
        enemy->vectorList[enemy->vectorListIndex].y - 1};

    if (position.x != -100)
    {
        if (enemy->interpolationValue > _interpolationMaxValue)
        {
            enemy->position.x = position.x;
            enemy->position.y = position.y;

            enemy->interpolationValue = 0;
            enemy->vectorListIndex += 1;
            enemy->isInterpolation = false;
        }
        else
        {
            enemy->isInterpolation = true;
            enemy->position.x = enemy->position.x + (position.x - enemy->position.x) * enemy->interpolationValue;
            enemy->position.y = enemy->position.y + (position.y - enemy->position.y) * enemy->interpolationValue;
            enemy->interpolationValue += 0.08f;
        }
    }
}