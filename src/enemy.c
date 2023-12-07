#include "includes/enemy.h"
#include "includes/pathfinding.h"

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
TINY_BURGER static uint32_t _collisionCenterX = TINY_BURGER_TILE / 4;
TINY_BURGER static bool _isCelebrating = false;

#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER static bool __equals_position(Vector2 a, Vector2 b);
    TINY_BURGER static void __init_vector_list(Enemy_t *enemy);
    TINY_BURGER static void __destroy_vector_list(Enemy_t *enemy);
    TINY_BURGER static void __update_enemy_path(Enemy_t *const enemy, const int32_t *const map, const Player_t *const player);
    TINY_BURGER static void __update_animation(Enemy_t *const enemy);
    TINY_BURGER static void __linear_interpolation(Enemy_t *const enemy);
    TINY_BURGER static AnimationPlayer_t *__init_animation_player(EnemyType_u type);
    TINY_BURGER static void __check_collision_player(Enemy_t *const enemy, Player_t *const player);
    TINY_BURGER static void __check_collision_bullet(Enemy_t *const enemy, const Player_t *const player);
    TINY_BURGER static void __reset_static_values(void);
    TINY_BURGER static void __update_dizzy(Enemy_t *const enemy);
    TINY_BURGER static void __falling_movement(Enemy_t *const enemy);

    TINY_BURGER static void __restarting(Enemy_t *const enemy);

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

    enemy->ap = __init_animation_player(type);
    if (enemy->ap == NULL)
    {
        MemFree(enemy);
        enemy = NULL;
        return enemy;
    }
    enemy->position = position;
    enemy->spawnPosition = position;
    enemy->interpolationPosition = position;
    enemy->playerPosition = (Vector2){0};
    enemy->type = type;
    enemy->interpolationValue = 0;
    enemy->internalFPSCounter = 0;
    enemy->interpolation = false;
    enemy->flipH = false;
    enemy->state = TB_ENEMY_STATE_FOLLOWING;
    enemy->collisionShape = (Rectangle){
        _collisionCenterX + position.x * TINY_BURGER_TILE,
        position.y * TINY_BURGER_TILE,
        TINY_BURGER_TILE / 2,
        TINY_BURGER_TILE,
    };

    __init_vector_list(enemy);
    __reset_static_values();
    TraceLog(LOG_DEBUG, "Enemy_t pointer created successfully.");
    return enemy;
}

TINY_BURGER void update_enemy(Enemy_t *const enemy, const int32_t *const map, Player_t *const player)
{
    switch (enemy->state)
    {
    case TB_ENEMY_STATE_RESTATING:
        __restarting(enemy);
        break;

    case TB_ENEMY_STATE_DIZZY:
        __update_dizzy(enemy);
        break;

    case TB_ENEMY_STATE_FALLING:
        __falling_movement(enemy);
        __linear_interpolation(enemy);
        __update_animation(enemy);
        break;
    case TB_ENEMY_STATE_FOLLOWING:
        if (!_isCelebrating)
        {
            __update_enemy_path(enemy, map, player);
            __check_collision_player(enemy, player);
            __check_collision_bullet(enemy, player);
            __linear_interpolation(enemy);
            __update_animation(enemy);
        }
        break;
    }
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
    if (!enemy->interpolation && !player->interpolation && !__equals_position(enemy->playerPosition, player->position))
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

TINY_BURGER static void __update_animation(Enemy_t *const enemy)
{
    if (enemy->interpolation)
    {
        EnemyAnimation_u animation = ENEMY_ANIMATION_RUN;

        uint32_t index = enemy->vectorListIndex;
        enemy->flipH = enemy->position.x < enemy->vectorList[index].x;
        if (enemy->position.y != (enemy->vectorList[index].y - 1))
            animation = ENEMY_ANIMATION_RUN_STAIR;

        if (_isCelebrating)
        {
            bool isStair = enemy->ap->currentId == ENEMY_ANIMATION_RUN_STAIR;
            animation = (isStair) ? ENEMY_ANIMATION_CELEBRATE_STAIR : ENEMY_ANIMATION_CELEBRATE;
            if (isStair)
                enemy->flipH = GetRandomValue(0, 1);
        }

        if (enemy->state == TB_ENEMY_STATE_FALLING)
        {
            animation = ENEMY_ANIMATION_FALLING;
        }

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
    Vector2 position = {0};
    if (enemy->state == TB_ENEMY_STATE_FALLING)
    {
        position = enemy->interpolationPosition;
    }
    else
    {
        position.x = enemy->vectorList[enemy->vectorListIndex].x;
        position.y = enemy->vectorList[enemy->vectorListIndex].y - 1;
    }

    if (position.x != -100)
    {
        if (enemy->interpolationValue > TINY_BURGER_INTERPOLATION_MAX_VALUE)
        {
            enemy->position = position;
            enemy->interpolationPosition = position;

            enemy->interpolationValue = 0;
            enemy->vectorListIndex += 1;
            enemy->interpolation = false;
        }
        else
        {
            enemy->interpolation = true;
            enemy->position.x = enemy->position.x + (position.x - enemy->position.x) * enemy->interpolationValue;
            enemy->position.y = enemy->position.y + (position.y - enemy->position.y) * enemy->interpolationValue;
            enemy->interpolationValue += 0.08f;
        }

        enemy->collisionShape.x = _collisionCenterX + enemy->position.x * TINY_BURGER_TILE;
        enemy->collisionShape.y = enemy->position.y * TINY_BURGER_TILE;
    }
}

TINY_BURGER static AnimationPlayer_t *__init_animation_player(EnemyType_u type)
{
    AnimationPlayer_t *ap = create_animation_player(TINY_BURGER_ENEMY_ANIMATION_LENGTH);
    if (ap != NULL)
    {
        uint32_t posY = type + 1;
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

        Rectangle celebrate[] = {
            (Rectangle){7 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){8 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){9 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        Rectangle dizzy[] = {
            (Rectangle){10 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){11 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        Rectangle falling[] = {
            (Rectangle){12 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){13 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        Rectangle celebrateStair[] = {
            (Rectangle){14 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
            (Rectangle){15 * TINY_BURGER_TILE, posY * TINY_BURGER_TILE, TINY_BURGER_TILE, TINY_BURGER_TILE},
        };

        add_frames_animation_player(ap, ENEMY_ANIMATION_IDLE, idle, 2);
        add_frames_animation_player(ap, ENEMY_ANIMATION_RUN, run, 2);
        add_frames_animation_player(ap, ENEMY_ANIMATION_IDLE_STAIR, idleStair, 1);
        add_frames_animation_player(ap, ENEMY_ANIMATION_IDLE_STAIR_MOV, idleStairUp, 1);
        add_frames_animation_player(ap, ENEMY_ANIMATION_RUN_STAIR, runUp, 2);
        add_frames_animation_player(ap, ENEMY_ANIMATION_CELEBRATE, celebrate, 3);
        add_frames_animation_player(ap, ENEMY_ANIMATION_DIZZY, dizzy, 2);
        add_frames_animation_player(ap, ENEMY_ANIMATION_FALLING, falling, 2);
        add_frames_animation_player(ap, ENEMY_ANIMATION_CELEBRATE_STAIR, celebrateStair, 2);
        set_animation_player(ap, ENEMY_ANIMATION_IDLE_STAIR);
        set_frame_add_animation_player(ap, 0.8f);
    }

    return ap;
}

TINY_BURGER static void __check_collision_player(Enemy_t *const enemy, Player_t *const player)
{
    bool collision = CheckCollisionRecs(enemy->collisionShape, player->collisionShape);
    if (!(enemy->state == TB_ENEMY_STATE_DIZZY) && collision)
    {
        set_falling_player(player, true);
        _isCelebrating = true;
    }
}

TINY_BURGER static void __check_collision_bullet(Enemy_t *const enemy, const Player_t *const player)
{
    for (size_t i = 0; i < BULLET_AMOUNT; i++)
    {
        Bullet_t *const bullet = player->gun->bulletList[i];
        bool collision = bullet->active && CheckCollisionRecs(enemy->collisionShape, bullet->rect);
        if (collision)
        {
            if (enemy->ap->currentId == ENEMY_ANIMATION_RUN_STAIR)
            {
                enemy->state = TB_ENEMY_STATE_FALLING;
            }
            else
            {
                enemy->state = TB_ENEMY_STATE_DIZZY;
                active_particle_bullet(bullet);
            }
        }
    }
}

// TODO: Remove function.
TINY_BURGER static void __reset_static_values(void)
{
    _isCelebrating = false;
}

TINY_BURGER static void __update_dizzy(Enemy_t *const enemy)
{
    set_animation_player(enemy->ap, ENEMY_ANIMATION_DIZZY);
    if (enemy->internalFPSCounter < TINY_BURGER_FPS * 3)
    {
        enemy->internalFPSCounter++;
    }
    else
    {
        enemy->internalFPSCounter = 0;
        enemy->state = TB_ENEMY_STATE_FOLLOWING;
    }
}

TINY_BURGER static void __falling_movement(Enemy_t *const enemy)
{
    if (!enemy->interpolation)
    {
        enemy->interpolationPosition.y += 1;
        enemy->interpolation = true;
        if (enemy->position.y >= TINY_BURGER_MAP_HEIGHT)
        {
            enemy->state = TB_ENEMY_STATE_RESTATING;
        }
    }
}

TINY_BURGER static void __restarting(Enemy_t *const enemy)
{
    if (enemy->internalFPSCounter > TINY_BURGER_FPS)
    {
        enemy->position = enemy->spawnPosition;
        enemy->playerPosition = (Vector2){0};
        enemy->internalFPSCounter = 0;
        enemy->interpolation = false;
        enemy->flipH = false;
        enemy->state = TB_ENEMY_STATE_FOLLOWING;
        set_animation_player(enemy->ap, ENEMY_ANIMATION_RUN);
    }
    else
    {
        enemy->internalFPSCounter++;
    }
}