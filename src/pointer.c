#include "includes/pointer.h"
#include "includes/package.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER Pointer_t *create_pointer(Vector2 position)
{
    Pointer_t *pointer = (Pointer_t *)MemAlloc(sizeof(Pointer_t));
    if (!pointer)
    {
        TraceLog(LOG_DEBUG, "Error to create Pointer_t pointer.");
        return NULL;
    }

    pointer->position = (Vector2){
        TINY_BURGER_WIDTH / 2 - 80,
        position.y};
    pointer->base = (Rectangle){
        TINY_BURGER_TILE * 5,
        TINY_BURGER_TILE * 5,
        6,
        9};
    pointer->dimension = (Vector2){
        5 * 2,
        9 * 2};

    return pointer;
}

TINY_BURGER void update_pointer(Pointer_t *const pointer, Vector2 position)
{
    pointer->position.y = position.y;
}

TINY_BURGER void draw_pointer(const Pointer_t *const pointer)
{
    DrawTexturePro(
        globalPackage->textures[TB_TEXTURE_TYPE_TILE],
        pointer->base,
        (Rectangle){
            pointer->position.x,
            pointer->position.y,
            pointer->dimension.x,
            pointer->dimension.y,
        },
        (Vector2){0},
        0.0f,
        RAYWHITE);
}

TINY_BURGER void destroy_pointer(Pointer_t **ptr)
{
    if ((*ptr))
    {
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Pointer_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
