#include "includes/logo.h"
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
TINY_BURGER Logo_t *create_logo(void)
{
    Logo_t *logo = MemAlloc(sizeof(Logo_t));
    if (!logo)
    {
        TraceLog(LOG_DEBUG, "Error to create Logo_t pointer.");
        return NULL;
    }

    logo->source = (Rectangle){
        0,
        TINY_BURGER_TILE * 6,
        TINY_BURGER_TILE * 6,
        TINY_BURGER_FONT_SIZE};

    logo->dest = (Rectangle){
        TINY_BURGER_WIDTH / 6.5,
        TINY_BURGER_HEIGHT / 5,
        TINY_BURGER_TILE * 6 * 5,
        TINY_BURGER_FONT_SIZE * 5};

    return logo;
}

TINY_BURGER void draw_logo(const Logo_t *const logo)
{
    DrawTexturePro(
        globalPackage->textures[TB_TEXTURE_TYPE_TILE],
        logo->source,
        logo->dest,
        (Vector2){0},
        0,
        RAYWHITE);
}

TINY_BURGER void destroy_logo(Logo_t **ptr)
{
    if ((*ptr))
    {
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Logo_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
