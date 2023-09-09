#include "includes/ingredient.h"
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
    TINY_BURGER static void __init_ingredient(Ingredient_t *const ingredient);
    TINY_BURGER static void __init_segment(Ingredient_t *const ingredient);
    TINY_BURGER static void __draw_ingredient(const Ingredient_t *const ingredient);
    TINY_BURGER static void __check_collision_ingredient(Ingredient_t *const ingredient, Rectangle collisionShape, bool isCollisionShape);
    TINY_BURGER static void __check_collision_ingredient_segment(Ingredient_t *const ingredient, Rectangle collisionShape);
#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER Ingredient_t *create_ingredient(IngredientType_u type, Vector2 position)
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
    __init_ingredient(ingredient);
    __init_segment(ingredient);
    return ingredient;
}

TINY_BURGER void update_ingredient(Ingredient_t *const ingredient, Rectangle collisionShape, bool isCollisionShape)
{
    __check_collision_ingredient(ingredient, collisionShape, isCollisionShape);
    __check_collision_ingredient_segment(ingredient, collisionShape);
}

// TODO: Delete isCollisionShape param.
TINY_BURGER void draw_ingredient(const Ingredient_t *const ingredient)
{
    __draw_ingredient(ingredient);
    // if (ingredient->isCollision)
    // {

    //     for (uint32_t i = 0; i < 3; ++i)
    //     {
    //         DrawRectangleLinesEx(
    //             (Rectangle){
    //                 ingredient->segment[i].position.x,
    //                 ingredient->segment[i].position.y,
    //                 TINY_BURGER_TILE,
    //                 TINY_BURGER_TILE,
    //             },
    //             1.0,
    //             GREEN);
    //     }
    // }
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
            ingredient->position.y * TINY_BURGER_TILE + (TINY_BURGER_MIDDLE_TILE),
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
        for (uint32_t index = 0; index < 3; ++index)
        {
            if (!ingredient->segment[index].check)
            {
                Rectangle shape = (Rectangle){
                    ingredient->segment[index].position.x,
                    ingredient->segment[index].position.y,
                    TINY_BURGER_TILE,
                    TINY_BURGER_TILE};
                ingredient->segment[index].check = CheckCollisionRecs(shape, collisionShape);
                if (ingredient->segment[index].check)
                {
                    ingredient->segment[index].position.y += 8;
                    break;
                }
            }
        }

        if (ingredient->segment[0].check && ingredient->segment[1].check && ingredient->segment[2].check)
        {
            TraceLog(LOG_INFO, ">>> COMPLETED");
        }
    }
}