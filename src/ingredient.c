#include "includes/ingredient.h"
#include "includes/package.h"
#include "includes/gui.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;
extern GuiData_t *globalGuiData;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER static void __init_ingredient(Ingredient_t *const ingredient);
    TINY_BURGER static void __init_segment(Ingredient_t *const ingredient);
    TINY_BURGER static void __draw_ingredient(const Ingredient_t *const ingredient);
    TINY_BURGER static void __check_collision_ingredient(Ingredient_t *const ingredient, Rectangle collisionShape, bool isCollisionShape);
    TINY_BURGER static void __check_collision_ingredient_segment(Ingredient_t *const ingredient, Rectangle collisionShape);
    TINY_BURGER static void __linear_interpolation(Ingredient_t *const ingredient, Vector2 position, bool isLast);

#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER Ingredient_t *create_ingredient(IngredientType_u type, Vector2 position, uint32_t pathIndex, uint32_t index)
{
    Ingredient_t *ingredient = (Ingredient_t *)MemAlloc(sizeof(Ingredient_t));
    if (ingredient == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Ingredient_t pointer.");
        return NULL;
    }

    ingredient->position = position;
    ingredient->type = type;
    ingredient->isCollision = false;
    ingredient->isCompleted = false;
    ingredient->time = 0.0f;
    ingredient->pathIndex = pathIndex;
    ingredient->index = index;

    __init_ingredient(ingredient);
    __init_segment(ingredient);

    return ingredient;
}

TINY_BURGER void check_collision_ingredient(Ingredient_t *const ingredient, Rectangle collisionShape, bool isCollisionShape)
{
    __check_collision_ingredient(ingredient, collisionShape, isCollisionShape);
    __check_collision_ingredient_segment(ingredient, collisionShape);
}

TINY_BURGER void transition_ingredient(Ingredient_t *const ingredient, Vector2 position, bool isLast)
{
    __linear_interpolation(ingredient, position, isLast);
}

TINY_BURGER void draw_ingredient(const Ingredient_t *const ingredient)
{
    __draw_ingredient(ingredient);
}

TINY_BURGER bool is_check_ingredient(const Ingredient_t *const ingredient)
{
    return ingredient->segment[0].check && ingredient->segment[1].check && ingredient->segment[2].check;
}

TINY_BURGER bool is_completed_ingredient(const Ingredient_t *const ingredient)
{
    return ingredient->isCompleted;
}

TINY_BURGER Rectangle get_collision_shape_ingredient(const Ingredient_t *const ingredient)
{
    return (Rectangle){
        ingredient->position.x * TINY_BURGER_TILE,
        ingredient->position.y * TINY_BURGER_TILE,
        TINY_BURGER_TILE * 3,
        TINY_BURGER_TILE};
}

TINY_BURGER uint32_t get_path_index_ingredient(const Ingredient_t *const ingredient)
{
    return ingredient->pathIndex;
}

TINY_BURGER void set_path_index_ingredient(Ingredient_t *const ingredient, uint32_t index)
{
    ingredient->pathIndex = index;
}

TINY_BURGER void set_segment_check_ingredient(Ingredient_t *const ingredient, bool value)
{
    ingredient->segment[0].check = value;
    ingredient->segment[1].check = value;
    ingredient->segment[2].check = value;
}

TINY_BURGER void destroy_ingredient(Ingredient_t **ptr)
{
    if ((*ptr) != NULL)
    {
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Ingredient_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __init_ingredient(Ingredient_t *const ingredient)
{
    switch (ingredient->type)
    {
    case TINY_BURGER_INGREDIENT_BREAD_UP:
        ingredient->source[0] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[1] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[2] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            -TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        break;

    case TINY_BURGER_INGREDIENT_BREAD_DOWN:
        ingredient->source[0] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            TINY_BURGER_TILE,
            -TINY_BURGER_MIDDLE_TILE};
        ingredient->source[1] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            -TINY_BURGER_MIDDLE_TILE};
        ingredient->source[2] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            -TINY_BURGER_TILE,
            -TINY_BURGER_MIDDLE_TILE};
        break;
    case TINY_BURGER_INGREDIENT_CHEESE:
        ingredient->source[0] = (Rectangle){
            TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[1] = (Rectangle){
            TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE,
            -TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[2] = (Rectangle){
            TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            -TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        break;

    case TINY_BURGER_INGREDIENT_MEAT:
        ingredient->source[0] = (Rectangle){
            2 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[1] = (Rectangle){
            2 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[2] = (Rectangle){
            2 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            -TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        break;

    case TINY_BURGER_INGREDIENT_SALAD:
        ingredient->source[0] = (Rectangle){
            3 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[1] = (Rectangle){
            3 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[2] = (Rectangle){
            3 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            -TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        break;
    case TINY_BURGER_INGREDIENT_TOMATO:
        ingredient->source[0] = (Rectangle){
            4 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[1] = (Rectangle){
            4 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        ingredient->source[2] = (Rectangle){
            4 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
            -TINY_BURGER_TILE,
            TINY_BURGER_MIDDLE_TILE};
        break;
    }
}

TINY_BURGER static void __init_segment(Ingredient_t *const ingredient)
{
    for (uint32_t i = 0; i < 3; ++i)
    {
        ingredient->segment[i].position = (Vector2){
            (ingredient->position.x * TINY_BURGER_TILE) + (TINY_BURGER_TILE * i),
            ingredient->position.y * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE,
        };
    }
}

TINY_BURGER static void __draw_ingredient(const Ingredient_t *const ingredient)
{
    for (uint32_t i = 0; i < 3; ++i)
    {
        DrawTextureRec(
            globalPackage->textures[TB_TEXTURE_TYPE_TILE],
            ingredient->source[i],
            ingredient->segment[i].position,
            RAYWHITE);
    }
}

TINY_BURGER static void __check_collision_ingredient(Ingredient_t *const ingredient, Rectangle collisionShape, bool isCollisionShape)
{
    if (isCollisionShape)
    {
        Rectangle shape = (Rectangle){
            ingredient->position.x * TINY_BURGER_TILE,
            ingredient->position.y * TINY_BURGER_TILE,
            TINY_BURGER_TILE * 3,
            TINY_BURGER_TILE};

        ingredient->isCollision = CheckCollisionRecs(shape, collisionShape);
    }
    else if (ingredient->isCollision)
    {
        ingredient->isCollision = false;
    }
}

TINY_BURGER static void __check_collision_ingredient_segment(Ingredient_t *const ingredient, Rectangle collisionShape)
{
    bool isCheck = ingredient->segment[0].check && ingredient->segment[1].check && ingredient->segment[2].check;
    if (ingredient->isCollision && !isCheck)
    {
        for (uint32_t i = 0; i < 3; ++i)
        {
            if (!ingredient->segment[i].check)
            {
                Rectangle shape = (Rectangle){
                    ingredient->segment[i].position.x,
                    ingredient->segment[i].position.y,
                    TINY_BURGER_TILE,
                    TINY_BURGER_TILE};

                if (CheckCollisionRecs(shape, collisionShape))
                {
                    ingredient->segment[i].check = true;
                    ingredient->segment[i].position.y += 4;
                    break;
                }
            }
        }

        if (ingredient->segment[0].check && ingredient->segment[1].check && ingredient->segment[2].check)
        {
            set_path_index_ingredient(ingredient, ingredient->pathIndex + 1);
        }
    }
}

TINY_BURGER static void __linear_interpolation(Ingredient_t *const ingredient, Vector2 position, bool isLast)
{
    for (uint32_t i = 0; i < 3; ++i)
    {
        float diff = isLast ? ingredient->index * TINY_BURGER_MIDDLE_TILE : 0;
        Vector2 cp = ingredient->segment[i].position;
        Vector2 np = (Vector2){
            position.x * TINY_BURGER_TILE + (TINY_BURGER_TILE * i),
            position.y * TINY_BURGER_TILE + TINY_BURGER_MIDDLE_TILE - diff,
        };
        ingredient->segment[i].position.x = cp.x + (np.x - cp.x) * ingredient->time;
        ingredient->segment[i].position.y = cp.y + (np.y - cp.y) * ingredient->time;
    }
    // --
    Vector2 cp = ingredient->position;
    Vector2 np = position;
    ingredient->position.x = cp.x + (np.x - cp.x) * ingredient->time;
    ingredient->position.y = cp.y + (np.y - cp.y) * ingredient->time;
    // --
    ingredient->time += 0.05f;
    if (ingredient->time > 1)
    {
        set_segment_check_ingredient(ingredient, false);
        ingredient->position = position;
        ingredient->isCollision = false;
        ingredient->isCompleted = isLast;
        ingredient->time = 0.0f;
        globalGuiData->currentPoints += 50;
    }
}