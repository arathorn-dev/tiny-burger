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
TINY_BURGER static int32_t _currentLevel = -1;
TINY_BURGER static Camera2D *_camera = NULL;
TINY_BURGER static bool _showPath = false;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER static void __draw_map(void);
    TINY_BURGER static void __load_level(int32_t level);
    TINY_BURGER static const char *__get_level_path(int32_t level);

#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------

TINY_BURGER Screen_t *create_game(void)
{
    Screen_t *screen = (Screen_t *)MemAlloc(sizeof(Screen_t *));
    if (screen == NULL)
    {
        TraceLog(LOG_DEBUG, "[GAME] Error to create a Screen_t pointer.");
        return NULL;
    }

    __load_level(0);

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
    else if (IsKeyPressed(KEY_F1))
        __load_level(0);
    else if (IsKeyPressed(KEY_F2))
        __load_level(1);
    else if (IsKeyPressed(KEY_F3))
        __load_level(2);
    else if (IsKeyPressed(KEY_F4))
        __load_level(3);
    else if (IsKeyPressed(KEY_F5))
        __load_level(4);
    else if (IsKeyPressed(KEY_F6))
        __load_level(5);
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
                    DrawRectangleLines(
                        position.x,
                        position.y - TINY_BURGER_TILE,
                        TINY_BURGER_TILE,
                        TINY_BURGER_TILE,
                        GetColor(TINY_BURGER_COLOR_9));
                }
            }
            position.x += TINY_BURGER_TILE;
        }
        position.x = 0;
        position.y += TINY_BURGER_TILE;
    }
}

TINY_BURGER static void __load_level(int32_t level)
{
    if (_currentLevel != level)
    {
        const char *fileName = __get_level_path(level);
        unload_draw_map(&_vectorDraw);
        unload_path_map(&_vectorPath);
        _vectorDraw = load_draw_map(fileName, TINY_BURGER_MAP_WIDTH, TINY_BURGER_MAP_HEIGHT);
        _vectorPath = load_path_map(_vectorDraw, TINY_BURGER_MAP_WIDTH, TINY_BURGER_MAP_HEIGHT);
        _currentLevel = level;
    }
}

TINY_BURGER static const char *__get_level_path(int32_t level)
{
    switch (level)
    {
    case 0:
        return "data/levels/level_0.csv";
    case 1:
        return "data/levels/level_1.csv";
    case 2:
        return "data/levels/level_2.csv";
    case 3:
        return "data/levels/level_3.csv";
    case 4:
        return "data/levels/level_4.csv";
    case 5:
        return "data/levels/level_5.csv";
    default:
        return NULL;
    }
}