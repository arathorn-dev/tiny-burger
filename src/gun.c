#include "includes/gun.h"
#include "includes/raylib.h"

TINY_BURGER Gun_t *create_gun(Vector2 position)
{
    Gun_t *gun = (Gun_t *)MemAlloc(sizeof(Gun_t));
    if (gun == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Gun_t pointer.");
        return NULL;
    }
    gun->position = position;
    TraceLog(LOG_DEBUG, "Gun_t pointer created successfully.");
    return gun;
}

TINY_BURGER void update_gun(Gun_t *const gun, Vector2 position)
{
    gun->position = position;
}

TINY_BURGER void draw_gun(const Gun_t *const gun)
{
    DrawLine(
    gun->position.x - 10,
    gun->position.y,
    gun->position.x + 10,
    gun->position.y,
    RED);

    DrawLine(
    gun->position.x,
    gun->position.y - 10,
    gun->position.x,
    gun->position.y + 10,
    GREEN);
}

TINY_BURGER void destroy_gun(Gun_t **ptr)
{
    if ((*ptr) != NULL) 
    {
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Gun_t pointer destroyed successfully.");
    }
}

