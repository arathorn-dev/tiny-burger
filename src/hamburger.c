#include "includes/hamburger.h"

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER static void __init_ingredient_vector(Hamburger_t *const hamburger);
    TINY_BURGER static void __update_ingredient_vector(Hamburger_t *const hamburger);
    TINY_BURGER static void __draw_ingredient_vector(const Hamburger_t *const hamburger);
    TINY_BURGER static void __destroy_ingredient_vector(Hamburger_t *const hamburger);
#if defined(__cplusplus)
}
#endif
//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER Hamburger_t *create_hamburger(void)
{
    Hamburger_t *hamburger = (Hamburger_t *)MemAlloc(sizeof(Hamburger_t));
    if (hamburger == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Hamburger_t pointer.");
        return NULL;
    }

    __init_ingredient_vector(hamburger);

    return hamburger;
}

TINY_BURGER void add_hamburger(Hamburger_t *const hamburger, IngredientType_u type, uint32_t index, Vector2 position)
{
    if (hamburger->ingredient[index] == NULL)
    {
        hamburger->ingredient[index] = create_ingredient(type, position);
    }
}

TINY_BURGER void update_hamburger(Hamburger_t *const hamburger)
{
    // TODO
}

TINY_BURGER void draw_hamburger(const Hamburger_t *const hamburger)
{
    __draw_ingredient_vector(hamburger);
}

TINY_BURGER void destroy_hamburger(Hamburger_t **ptr)
{
    if ((*ptr) != NULL)
    {
        __destroy_ingredient_vector((*ptr));
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Hamburger_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __init_ingredient_vector(Hamburger_t *const hamburger)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_INGREDIENT_SIZE; ++i)
        hamburger->ingredient[i] = NULL;
}

TINY_BURGER static void __update_ingredient_vector(Hamburger_t *const hamburger)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_INGREDIENT_SIZE; ++i)
    {
        if (hamburger->ingredient[i] != NULL)
        {
            update_ingredient(hamburger->ingredient[i]);
        }
    }
}

TINY_BURGER static void __draw_ingredient_vector(const Hamburger_t *const hamburger)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_INGREDIENT_SIZE; ++i)
    {
        if (hamburger->ingredient[i] != NULL)
        {
            draw_ingredient(hamburger->ingredient[i]);
        }
    }
}

TINY_BURGER static void __destroy_ingredient_vector(Hamburger_t *const hamburger)
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