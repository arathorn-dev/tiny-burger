#include "includes/bullet.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
TINY_BURGER static float _velocity = 4;
TINY_BURGER static uint32_t _widthLimit = TINY_BURGER_MAP_WIDTH * TINY_BURGER_TILE;

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------

TINY_BURGER Bullet_t *create_bullet(Vector2 position, bool right)
{
    Bullet_t *bullet = (Bullet_t *)MemAlloc(sizeof(Bullet_t));
    if (bullet == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Bullet_t pointer.");
        return NULL;
    }

    bullet->particleList = create_particle_list(30);
    if (bullet->particleList == NULL)
    {
        MemFree(bullet);
        bullet = NULL;
        return NULL;
    }

    bullet->rect = (Rectangle){
        position.x,
        position.y,
        4,
        2,
    };
    bullet->right = right;
    bullet->active = false;
    bullet->color = GetColor(TINY_BURGER_COLOR_15);

    TraceLog(LOG_DEBUG, "Bullet_t pointer created successfully.");
    return bullet;
}

TINY_BURGER void update_bullet(Bullet_t *const bullet, Vector2 position)
{
    if (bullet->active)
    {
        bullet->rect.x += _velocity * ((bullet->right) ? 1 : -1);
        bullet->active = !(bullet->rect.x < 0 || bullet->rect.x >= _widthLimit);
    }
    else
    {
        bullet->rect.x = position.x;
        bullet->rect.y = position.y;
    }
    update_particle_list(bullet->particleList);
}

TINY_BURGER void draw_bullet(const Bullet_t *const bullet)
{
    if (bullet->active)
    {
        DrawRectangleRec(bullet->rect, bullet->color);
    }
    draw_particle_list(bullet->particleList);
}

TINY_BURGER void set_active_bullet(Bullet_t *bullet, bool right)
{
    bullet->active = true;
    bullet->right = right;
}

TINY_BURGER void active_particle_bullet(Bullet_t *const bullet)
{
    bullet->active = false;
    active_particle_list(bullet->particleList, (Vector2){bullet->rect.x, bullet->rect.y});
}

TINY_BURGER void destroy_bullet(Bullet_t **ptr)
{
    if ((*ptr) != NULL)
    {
        destroy_particle_list(&(*ptr)->particleList);
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Bullet_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------