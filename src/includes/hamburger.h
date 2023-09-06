#ifndef __TINY_BURGER_HAMBURGER_H__
#define __TINY_BURGER_HAMBURGER_H__

#include "config.h"
#include "ingredient.h"

TINY_BURGER typedef struct
{
    Ingredient_t *ingredient[TINY_BURGER_MAX_INGREDIENT_SIZE];
    Rectangle collisionShape;
    bool isCollision;

} Hamburger_t;

#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER Hamburger_t *create_hamburger(Rectangle collisionShape);
    TINY_BURGER void add_hamburger(Hamburger_t *const hamburger, IngredientType_u type, uint32_t index, Vector2 position);
    TINY_BURGER void update_hamburger(Hamburger_t *const hamburger, Rectangle collisionShape);
    TINY_BURGER void draw_hamburger(const Hamburger_t *const hamburger);
    TINY_BURGER void destroy_hamburger(Hamburger_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_HAMBURGER_H__