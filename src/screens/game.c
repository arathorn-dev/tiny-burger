#include "../includes/screen.h"
#include "../includes/utils.h"
#include "../includes/package.h"
#include "../includes/player.h"
#include "../includes/enemy.h"

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

TINY_BURGER static bool _showPath = false;

TINY_BURGER static Camera2D *_camera = NULL;
TINY_BURGER static Player_t *_player = NULL;
TINY_BURGER static Enemy_t *_enemy = NULL;

#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER static void __draw_map(void);
    TINY_BURGER static bool __load_level(int32_t level);
    TINY_BURGER static const char *__get_level_path(int32_t level);
    TINY_BURGER static Vector2 __get_position_player(int32_t level);
    TINY_BURGER static Vector2 __get_position_enemy(int32_t level);

#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------

TINY_BURGER Screen_t *create_game(void)
{
    Screen_t *screen = (Screen_t *)MemAlloc(sizeof(Screen_t));
    if (screen == NULL)
    {
        TraceLog(LOG_DEBUG, "[GAME] Error to create a Screen_t pointer.");
        return NULL;
    }

    if (!__load_level(0))
    {
        MemFree(screen);
        screen = NULL;
        return screen;
    }

    _camera = MemAlloc(sizeof(Camera2D));
    _camera->target = (Vector2){0};
    _camera->offset = (Vector2){48, 68};
    _camera->rotation = 0.0f;
    _camera->zoom = 2.0f;

    screen->currentScreenType = TB_SCREEN_TYPE_GAME;
    screen->nextScreenType = TB_SCREEN_TYPE_EMPTY;
    screen->background = GetColor(TINY_BURGER_COLOR_0);

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

    update_player(_player, _vectorPath);
    update_enemy(_enemy, _vectorPath, _player);
}
TINY_BURGER void draw_game(const Screen_t *const screen)
{
    DrawFPS(0, 0);
    BeginMode2D(*_camera);
    DrawRectangle(0, 0, TINY_BURGER_WIDTH, TINY_BURGER_HEIGHT, screen->background);
    __draw_map();
    draw_player(_player);
    draw_enemy(_enemy);
    EndMode2D();
}

TINY_BURGER void destroy_game(Screen_t **ptr)
{
    if ((*ptr) != NULL)
    {
        destroy_player(&_player);
        destroy_enemy(&_enemy);
        unload_path_map(&_vectorPath);
        unload_draw_map(&_vectorDraw);
        MemFree(_camera);
        _camera = NULL;
        _vectorDraw = NULL;
        _vectorPath = NULL;
        _player = NULL;

        _currentLevel = -1;
        _showPath = false;

        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "[GAME] Screen_t pointer destroyed successfully.");
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
                (Rectangle){TINY_BURGER_TILE * v, 0, TINY_BURGER_TILE, TINY_BURGER_TILE},
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

TINY_BURGER static bool __load_level(int32_t level)
{
    bool isLoaded = false;
    if (_currentLevel != level)
    {
        unload_draw_map(&_vectorDraw);
        unload_path_map(&_vectorPath);
        destroy_player(&_player);
        destroy_enemy(&_enemy);
        _player = create_player(__get_position_player(level));
        if (_player != NULL)
        {
            const char *fileName = __get_level_path(level);
            _vectorDraw = load_draw_map(fileName, TINY_BURGER_MAP_WIDTH, TINY_BURGER_MAP_HEIGHT);
            _vectorPath = load_path_map(_vectorDraw, TINY_BURGER_MAP_WIDTH, TINY_BURGER_MAP_HEIGHT);
            _enemy = create_enemy(TB_ENEMY_TYPE_EGG, __get_position_enemy(level)); // TODO: Remove this.
            _currentLevel = level;
            isLoaded = true;
        }
    }
    return isLoaded;
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

TINY_BURGER static Vector2 __get_position_player(int32_t level)
{
    Vector2 position = (Vector2){0};
    switch (level)
    {
    case 0:
        position = (Vector2){8, 8};
        break;
    case 1:
        position = (Vector2){8, 9};
        break;
    case 2:
        position = (Vector2){8, 10};
        break;
    case 3:
        position = (Vector2){8, 6};
        break;
    case 4:
        // position = (Vector2){12, 10};
        // position = (Vector2){13, -1};
        position = (Vector2){10, 1};
        break;
    case 5:
        position = (Vector2){8, 8};
        break;
    }
    return position;
}

TINY_BURGER static Vector2 __get_position_enemy(int32_t level)
{
    Vector2 position = (Vector2){0};
    switch (level)
    {
    case 0:
        position = (Vector2){15, 1};
        break;
    case 1:
        position = (Vector2){15, 1};
        break;
    case 2:
        position = (Vector2){14, 0};
        break;
    case 3:
        position = (Vector2){15, 0};
        break;
    case 4:
        position = (Vector2){1, 0};
        break;
    case 5:
        position = (Vector2){1, 0};
        break;
    }
    return position;
}
