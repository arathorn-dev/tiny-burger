#ifndef __TINY_BURGER_INGREDIENT_H__
#define __TINY_BURGER_INGREDIENT_H__

#include "config.h"

TINY_BURGER typedef enum {
    TINY_BURGER_INGREDIENT_BREAD_UP = 0,
    TINY_BURGER_INGREDIENT_BREAD_DOWN,
    TINY_BURGER_INGREDIENT_CHEESE,
    TINY_BURGER_INGREDIENT_MEAT,
    TINY_BURGER_INGREDIENT_SALAD,
    TINY_BURGER_INGREDIENT_TOMATO,
} IngredientType_u;

TINY_BURGER typedef struct
{
    Vector2 position;
    bool check;
} IngredientSegment_t;

TINY_BURGER typedef struct
{
    Vector2 position;
    IngredientType_u type;
    Rectangle source[3];
    IngredientSegment_t segment[3];
    bool isCollision;
} Ingredient_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Ingredient_t *create_ingredient(IngredientType_u type, Vector2 position);
    TINY_BURGER void update_ingredient(Ingredient_t *const ingredient, Rectangle collisionShape, bool isCollisionShape);
    TINY_BURGER void draw_ingredient(const Ingredient_t *const ingredient);
    TINY_BURGER void destroy_ingredient(Ingredient_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif //__TINY_BURGER_INGREDIENT_H__
