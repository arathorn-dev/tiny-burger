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
    TINY_BURGER static void __draw_ingredient(const Ingredient_t *const ingredient);
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
    __init_ingredient(ingredient);
    return ingredient;
}

TINY_BURGER void update_ingredient(Ingredient_t *const ingredient)
{
    // TODO
}

TINY_BURGER void draw_ingredient(const Ingredient_t *const ingredient)
{
    __draw_ingredient(ingredient);
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
    int32_t diff = 2;
    switch (ingredient->type)
    {
    case TINY_BURGER_INGREDIENT_BREAD_UP:
        ingredient->rect[0] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE + (TINY_BURGER_TILE / 2),
            TINY_BURGER_TILE,
            (TINY_BURGER_TILE / 2)};
        ingredient->rect[1] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            (TINY_BURGER_TILE / 2)};
        ingredient->rect[2] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE + (TINY_BURGER_TILE / 2),
            -TINY_BURGER_TILE,
            (TINY_BURGER_TILE / 2)};
        break;

    case TINY_BURGER_INGREDIENT_BREAD_DOWN:
        ingredient->rect[0] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            TINY_BURGER_TILE,
            -TINY_BURGER_TILE / 2};
        ingredient->rect[1] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            -TINY_BURGER_TILE / 2};
        ingredient->rect[2] = (Rectangle){
            0,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            -TINY_BURGER_TILE,
            -TINY_BURGER_TILE / 2};
        break;
    case TINY_BURGER_INGREDIENT_CHEESE:
        ingredient->rect[0] = (Rectangle){
            TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        ingredient->rect[1] = (Rectangle){
            TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE,
            -TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        ingredient->rect[2] = (Rectangle){
            TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            -TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        break;

    case TINY_BURGER_INGREDIENT_MEAT:
        ingredient->rect[0] = (Rectangle){
            2 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        ingredient->rect[1] = (Rectangle){
            2 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        ingredient->rect[2] = (Rectangle){
            2 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            -TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        break;

    case TINY_BURGER_INGREDIENT_SALAD:
        ingredient->rect[0] = (Rectangle){
            3 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        ingredient->rect[1] = (Rectangle){
            3 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        ingredient->rect[2] = (Rectangle){
            3 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            -TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        break;
    case TINY_BURGER_INGREDIENT_TOMATO:
        ingredient->rect[0] = (Rectangle){
            4 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        ingredient->rect[1] = (Rectangle){
            4 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE,
            TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        ingredient->rect[2] = (Rectangle){
            4 * TINY_BURGER_TILE,
            5 * TINY_BURGER_TILE + TINY_BURGER_TILE / 2,
            -TINY_BURGER_TILE,
            TINY_BURGER_TILE / 2};
        break;
    }
}

TINY_BURGER static void __draw_ingredient(const Ingredient_t *const ingredient)
{
    uint32_t middle = (TINY_BURGER_TILE / 2);
    for (uint32_t i = 0; i < 3; ++i)
    {
        DrawTextureRec(
            globalPackage->textures[TB_TEXTURE_TYPE_TILE],
            ingredient->rect[i],
            (Vector2){
                // TINY_BURGER_TILE * (ingredient->position.x * i * TINY_BURGER_TILE),
                (ingredient->position.x * TINY_BURGER_TILE) + (TINY_BURGER_TILE * i),
                ingredient->position.y * TINY_BURGER_TILE + middle,
            },
            RAYWHITE);
    }
}