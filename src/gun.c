#include "includes/gun.h"
#include "includes/raylib.h"
#include "includes/gui.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern GuiData_t *globalGuiData;
extern bool globalIsCollisionDebug;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
TINY_BURGER static size_t _fpsCounter = TINY_BURGER_FPS;
TINY_BURGER static size_t _fpsMax = TINY_BURGER_FPS / 2;
TINY_BURGER static size_t _bulletIndex = 0;

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------

TINY_BURGER Gun_t *create_gun(Vector2 position)
{
    Gun_t *gun = (Gun_t *)MemAlloc(sizeof(Gun_t));
    if (gun == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Gun_t pointer.");
        return NULL;
    }
    gun->position = position;

    for (size_t i = 0; i < BULLET_AMOUNT; i++)
    {
        gun->bulletList[i] = create_bullet(position, false);
    }

    TraceLog(LOG_DEBUG, "Gun_t pointer created successfully.");
    return gun;
}

TINY_BURGER void update_gun(Gun_t *const gun, bool isShooting, bool right, Vector2 position)
{
    // TODO: Change this.
    gun->position = (Vector2){
        position.x * TINY_BURGER_TILE + (right ? 16 : 0),
        position.y * TINY_BURGER_TILE + 12};

    if (globalGuiData->bulletAmount > 0 && isShooting)
    {
        if (_fpsCounter < _fpsMax)
        {
            _fpsCounter++;
        }
        else
        {
            set_active_bullet(gun->bulletList[_bulletIndex], right);
            _bulletIndex = ((_bulletIndex + 1) < BULLET_AMOUNT) ? _bulletIndex + 1 : 0;

            globalGuiData->bulletAmount--;
            _fpsCounter = 0;
        }
    }
    else if (_fpsCounter != TINY_BURGER_FPS)
    {
        _fpsCounter = TINY_BURGER_FPS;
    }

    for (size_t i = 0; i < BULLET_AMOUNT; ++i)
    {
        update_bullet(gun->bulletList[i], gun->position);
    }
}

TINY_BURGER void draw_gun(const Gun_t *const gun)
{
    if (globalIsCollisionDebug)
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

    for (size_t i = 0; i < BULLET_AMOUNT; ++i)
    {
        draw_bullet(gun->bulletList[i]);
    }
}

TINY_BURGER void destroy_gun(Gun_t **ptr)
{
    if ((*ptr) != NULL)
    {
        for (size_t i = 0; i < BULLET_AMOUNT; ++i)
        {
            destroy_bullet(&(*ptr)->bulletList[i]);
            (*ptr)->bulletList[i] = NULL;
        }

        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Gun_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------