#include "includes/hamburger.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern bool globalIsCollisionDebug;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER static void __init_ingredient(Hamburger_t *const hamburger);
    TINY_BURGER static void __update_ingredient(Hamburger_t *const hamburger, Rectangle collisionShape);
    TINY_BURGER static void __draw_ingredient(const Hamburger_t *const hamburger);
    TINY_BURGER static void __destroy_ingredient(Hamburger_t *const hamburger);
    TINY_BURGER static void __destroy_ingredient_path(Hamburger_t *const hamburger);
#if defined(__cplusplus)
}
#endif
//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER Hamburger_t *create_hamburger(Rectangle collisionShape, IngredientPath_t *path)
{
    Hamburger_t *hamburger = (Hamburger_t *)MemAlloc(sizeof(Hamburger_t));
    if (hamburger == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Hamburger_t pointer.");
        return NULL;
    }

    __init_ingredient(hamburger);
    hamburger->collisionShape = collisionShape;
    hamburger->isCollision = false;
    hamburger->isCompleted = false;
    hamburger->path = path;
    hamburger->indexIngredient = 0;

    return hamburger;
}

TINY_BURGER void add_hamburger(Hamburger_t *const hamburger, IngredientType_u type, uint32_t pathIndex, uint32_t index)
{
    if (hamburger->ingredient[hamburger->indexIngredient] == NULL)
    {
        Vector2 position = hamburger->path->vector[pathIndex];
        hamburger->ingredient[hamburger->indexIngredient] = create_ingredient(type, position, pathIndex, index);
        hamburger->indexIngredient += 1;
    }
}

TINY_BURGER void update_hamburger(Hamburger_t *const hamburger, Rectangle collisionShape)
{
    hamburger->isCollision = CheckCollisionRecs(
        hamburger->collisionShape,
        collisionShape);

    __update_ingredient(hamburger, collisionShape);
}

TINY_BURGER void draw_hamburger(const Hamburger_t *const hamburger)
{
    __draw_ingredient(hamburger);
    // if (globalIsCollisionDebug)
    // {
    //     DrawRectangleRec(
    //         hamburger->collisionShape,
    //         (hamburger->isCollision) ? GREEN : GRAY);
    // }
}

TINY_BURGER void destroy_hamburger(Hamburger_t **ptr)
{
    if ((*ptr) != NULL)
    {
        __destroy_ingredient_path((*ptr));
        __destroy_ingredient((*ptr));
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Hamburger_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __init_ingredient(Hamburger_t *const hamburger)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_INGREDIENT_SIZE; ++i)
        hamburger->ingredient[i] = NULL;
}

TINY_BURGER static void __update_ingredient(Hamburger_t *const hamburger, Rectangle collisionShape)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_INGREDIENT_SIZE; ++i)
    {
        if (hamburger->ingredient[i] != NULL)
        {
            if (is_completed_ingredient(hamburger->ingredient[i]))
            {
                uint32_t pathIndex = get_path_index_ingredient(hamburger->ingredient[i]);
                bool isLast = pathIndex == (hamburger->path->size - 1);
                transition_ingredient(hamburger->ingredient[i], hamburger->path->vector[pathIndex], isLast);
                // ---
                if ((i + 1) < TINY_BURGER_MAX_INGREDIENT_SIZE && hamburger->ingredient[i + 1] != NULL)
                {
                    uint32_t nextPathIndex = get_path_index_ingredient(hamburger->ingredient[i + 1]);
                    if (pathIndex == nextPathIndex)
                    {
                        Rectangle rec0 = get_collision_shape_ingredient(hamburger->ingredient[i]);
                        Rectangle rec1 = get_collision_shape_ingredient(hamburger->ingredient[i + 1]);
                        if ((nextPathIndex + 1) < hamburger->path->size && CheckCollisionRecs(rec0, rec1))
                        {
                            set_segment_check_ingredient(hamburger->ingredient[i + 1], true);
                            set_path_index_ingredient(hamburger->ingredient[i + 1], nextPathIndex + 1);
                        }
                    }
                }
                // ---
            }
            else
            {
                check_collision_ingredient(hamburger->ingredient[i], collisionShape, hamburger->isCollision);
            }
        }
    }
}

TINY_BURGER static void __draw_ingredient(const Hamburger_t *const hamburger)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_INGREDIENT_SIZE; ++i)
    {
        if (hamburger->ingredient[i] != NULL)
        {
            draw_ingredient(hamburger->ingredient[i]);
        }
    }
}

TINY_BURGER static void __destroy_ingredient(Hamburger_t *const hamburger)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_INGREDIENT_SIZE; ++i)
    {
        if (hamburger->ingredient[i] != NULL)
        {
            destroy_ingredient(&(hamburger->ingredient[i]));
            hamburger->ingredient[i] = NULL;
        }
    }
}

TINY_BURGER static void __destroy_ingredient_path(Hamburger_t *const hamburger)
{
    if (hamburger->path != NULL)
    {
        MemFree(hamburger->path->vector);
        hamburger->path->vector = NULL;
        MemFree(hamburger->path);
        hamburger->path = NULL;
        TraceLog(LOG_DEBUG, "IngredientPath_t pointer destroyed successfully.");
    }
}