#include "../includes/screen.h"
#include "../includes/utils.h"
#include "../includes/package.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

TINY_BURGER static int32_t *_vectorDraw = NULL;
TINY_BURGER static int32_t *_vectorPath = NULL;
TINY_BURGER static Camera2D *_camera = NULL;
TINY_BURGER static bool _showPath = false;

TINY_BURGER static void __draw_map(void);

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------

TINY_BURGER Screen_t *create_game(const char *fileName)
{
    Screen_t *screen = (Screen_t *)MemAlloc(sizeof(Screen_t *));
    if (screen == NULL)
    {
        TraceLog(LOG_DEBUG, "[GAME] Error to create a Screen_t pointer.");
        return NULL;
    }

    _vectorDraw = load_draw_map(fileName, TINY_BURGER_MAP_WIDTH, TINY_BURGER_MAP_HEIGHT);
    _vectorPath = load_path_map(_vectorDraw, TINY_BURGER_MAP_WIDTH, TINY_BURGER_MAP_HEIGHT);

    _camera = MemAlloc(sizeof(Camera2D));
    _camera->target = (Vector2){0};
    _camera->offset = (Vector2){48, 68};
    _camera->rotation = 0.0f;
    _camera->zoom = 4.0f;

    screen->currentScreenType = TB_SCREEN_TYPE_GAME;
    screen->nextScreenType = TB_SCREEN_TYPE_EMPTY;

    TraceLog(LOG_DEBUG, "[GAME] Screen_t pointer created successfully.");
    return screen;
}

TINY_BURGER void update_game(Screen_t *const screen)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        screen->nextScreenType = TB_SCREEN_TYPE_MENU;
    }
    else if (IsKeyPressed(KEY_F9))
    {
        _showPath = !_showPath;
    }
}
TINY_BURGER void draw_game(const Screen_t *const screen)
{
    BeginMode2D(*_camera);
    DrawRectangle(0, 0, TINY_BURGER_WIDTH, TINY_BURGER_HEIGHT, GetColor(TINY_BURGER_COLOR_0));
    __draw_map();
    EndMode2D();
}

TINY_BURGER void destroy_game(Screen_t **ptr)
{
    if ((*ptr) != NULL)
    {
        unload_path_map(&_vectorPath);
        unload_draw_map(&_vectorDraw);
        MemFree(_camera);
        _camera = NULL;

        TraceLog(LOG_DEBUG, "[GAME] Screen_t pointer destroyed successfully.");
        MemFree((*ptr));
        (*ptr) = NULL;
    }
}
//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __draw_map(void)
{
    Vector2 position = (Vector2){0};

    for (size_t i = 0; i < TINY_BURGER_MAP_HEIGHT; ++i)
    {
        for (size_t j = 0; j < TINY_BURGER_MAP_WIDTH; ++j)
        {
            int32_t v = _vectorDraw[j + i * TINY_BURGER_MAP_WIDTH];
            DrawTextureRec(
                globalPackage->textures[TB_TEXTURE_TYPE_TILE],
                (Rectangle){8 * v, 0, TINY_BURGER_TILE, TINY_BURGER_TILE},
                position,
                RAYWHITE);

            if (_showPath)
            {
                v = _vectorPath[j + i * TINY_BURGER_MAP_WIDTH];
                if (v > 0)
                {
                    DrawRectangle(position.x, position.y, TINY_BURGER_TILE, TINY_BURGER_TILE, GREEN);
                }
            }
            position.x += TINY_BURGER_TILE;
        }
        position.x = 0;
        position.y += TINY_BURGER_TILE;
    }
}
