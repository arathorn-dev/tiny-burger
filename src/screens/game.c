#include "../includes/screen.h"
#include "../includes/utils.h"
#include "../includes/package.h"
#include "../includes/player.h"
#include "../includes/enemy.h"
#include "../includes/hamburger.h"
#include "../includes/gui.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;
extern bool globalIsCollisionDebug;

GuiData_t *globalGuiData = NULL;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

TINY_BURGER static int32_t *_vectorDraw = NULL;
TINY_BURGER static int32_t *_vectorPath = NULL;
TINY_BURGER static int32_t _currentLevel = -1;

TINY_BURGER static bool _isHamburgerCompletedByIndex[TINY_BURGER_MAX_HAMBURGER_SIZE];
TINY_BURGER static uint32_t _maxHamburger = 0;
TINY_BURGER static uint32_t _countHamburger = 0;

TINY_BURGER static Camera2D *_camera = NULL;
TINY_BURGER static Player_t *_player = NULL;
TINY_BURGER static Gui_t *_gui = NULL;
// TINY_BURGER static Enemy_t *_enemy = NULL;
TINY_BURGER static Hamburger_t *_hamburger[TINY_BURGER_MAX_HAMBURGER_SIZE];

TINY_BURGER static bool _isLoading = true;
TINY_BURGER static uint32_t _fpsLoading = 0;
TINY_BURGER static uint32_t _countLoading = 0;
TINY_BURGER static uint32_t _maxLoading = 2;

#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER static void __destroy_screen(Screen_t **ptr);
    TINY_BURGER static void __draw_map(void);
    TINY_BURGER static bool __load_level(int32_t level);
    TINY_BURGER static const char *__get_level_path(int32_t level);
    TINY_BURGER static Vector2 __get_position_player(int32_t level);
    TINY_BURGER static Vector2 __get_position_enemy(int32_t level);

    TINY_BURGER static void __reset_loading(void);
    TINY_BURGER static void __draw_loading(void);
    TINY_BURGER static void __update_loading(void);

    TINY_BURGER static void __draw(const Screen_t *const screen);
    TINY_BURGER static void __update(Screen_t *const screen);

    TINY_BURGER static void __init_hamburger(void);
    TINY_BURGER static void __load_hamburger(int32_t level);
    TINY_BURGER static void __update_hamburger(void);
    TINY_BURGER static void __draw_hamburger(void);
    TINY_BURGER static void __unload_hamburger(void);

    TINY_BURGER static void __loadl_level0(void);
    TINY_BURGER static void __loadl_level1(void);
    TINY_BURGER static void __loadl_level2(void);
    TINY_BURGER static void __loadl_level3(void);
    TINY_BURGER static void __loadl_level4(void);
    TINY_BURGER static void __loadl_level5(void);

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

    globalGuiData = create_gui_data();
    if (!globalGuiData)
    {
        __destroy_screen(&screen);
        return NULL;
    }

    _gui = create_gui();
    if (!_gui)
    {
        destroy_gui_data(&globalGuiData);
        __destroy_screen(&screen);
        return NULL;
    }

    if (!__load_level(0))
    {
        destroy_gui_data(&globalGuiData);
        destroy_gui(&_gui);
        __destroy_screen(&screen);
        return NULL;
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
    if (_isLoading)
        __update_loading();
    else
        __update(screen);
}
TINY_BURGER void draw_game(const Screen_t *const screen)
{

    if (_isLoading)
        __draw_loading();
    else
        __draw(screen);
}

TINY_BURGER void destroy_game(Screen_t **ptr)
{
    if ((*ptr) != NULL)
    {
        destroy_gui(&_gui);
        destroy_gui_data(&globalGuiData);
        __unload_hamburger();
        destroy_player(&_player);
        // destroy_enemy(&_enemy);
        unload_path_map(&_vectorPath);
        unload_draw_map(&_vectorDraw);
        MemFree(_camera);
        _camera = NULL;
        _vectorDraw = NULL;
        _vectorPath = NULL;
        _player = NULL;
        _currentLevel = -1;

        for (uint32_t i = 0; i < TINY_BURGER_MAX_HAMBURGER_SIZE; ++i)
            _isHamburgerCompletedByIndex[i] = false;
        _maxHamburger = 0;
        _countHamburger = 0;

        __destroy_screen(ptr);
    }
}
//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __destroy_screen(Screen_t **ptr)
{
    if ((*ptr) != NULL)
    {
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "[GAME] Screen_t pointer destroyed successfully.");
    }
}

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

            if (globalIsCollisionDebug)
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
        __reset_loading();
        __unload_hamburger();
        unload_draw_map(&_vectorDraw);
        unload_path_map(&_vectorPath);
        destroy_player(&_player);
        // destroy_enemy(&_enemy);
        _maxHamburger = 0;
        _countHamburger = 0;
        if (globalGuiData != NULL)
            globalGuiData->levelCounter = level + 1;
        _player = create_player(__get_position_player(level));
        if (_player != NULL)
        {
            const char *fileName = __get_level_path(level);
            _vectorDraw = load_draw_map(fileName, TINY_BURGER_MAP_WIDTH, TINY_BURGER_MAP_HEIGHT);
            _vectorPath = load_path_map(_vectorDraw, TINY_BURGER_MAP_WIDTH, TINY_BURGER_MAP_HEIGHT);
            // _enemy = create_enemy(TB_ENEMY_TYPE_EGG, __get_position_enemy(level)); // TODO: Remove this.
            __init_hamburger();
            __load_hamburger(level);

            _currentLevel = level;
            isLoaded = true;
            for (uint32_t i = 0; i < TINY_BURGER_MAX_HAMBURGER_SIZE; ++i)
                _isHamburgerCompletedByIndex[i] = false;
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
        position = (Vector2){12, 10};
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

TINY_BURGER static void __reset_loading(void)
{
    _isLoading = true;
    _fpsLoading = 0;
    _countLoading = 0;
}

// TODO: Improve the function code.
TINY_BURGER static void __draw_loading(void)
{
    DrawTextEx(
        globalPackage->fonts[TB_FONT_TYPE_04B03],
        "PLAYER 1",
        (Vector2){(TINY_BURGER_WIDTH / 2) - MeasureText("PLAYER 1", TINY_BURGER_FONT_SIZE) / 2, TINY_BURGER_HEIGHT / 2},
        TINY_BURGER_FONT_SIZE,
        1,
        GetColor(TINY_BURGER_COLOR_5));

    DrawTextEx(
        globalPackage->fonts[TB_FONT_TYPE_04B03],
        "READY",
        (Vector2){(TINY_BURGER_WIDTH / 2) - MeasureText("READY", TINY_BURGER_FONT_SIZE) / 2, TINY_BURGER_HEIGHT / 2 + 32},
        TINY_BURGER_FONT_SIZE,
        1,
        GetColor(TINY_BURGER_COLOR_5));
}

TINY_BURGER static void __update_loading(void)
{
    if (_fpsLoading >= TINY_BURGER_FPS)
    {
        _countLoading++;
        _fpsLoading = 0;
    }
    else
        _fpsLoading++;

    if (_countLoading > _maxLoading)
    {
        _isLoading = false;
    }
}

TINY_BURGER static void __draw(const Screen_t *const screen)
{
    // DrawFPS(0, 0);
    draw_gui(_gui);
    BeginMode2D(*_camera);
    DrawRectangle(0, 0, TINY_BURGER_WIDTH, TINY_BURGER_HEIGHT, screen->background);
    __draw_map();
    draw_player(_player);
    __draw_hamburger();
    // draw_enemy(_enemy);
    EndMode2D();
}

TINY_BURGER static void __update(Screen_t *const screen)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        screen->nextScreenType = TB_SCREEN_TYPE_MENU;
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

    // ----
    if (_countHamburger >= _maxHamburger && (_currentLevel + 1) < 6)
    {
        __load_level(_currentLevel + 1);
    }

    // ----

    update_gui(_gui);
    __update_hamburger();
    update_player(_player, _vectorPath);
    // update_enemy(_enemy, _vectorPath, _player);
}

TINY_BURGER static void __init_hamburger(void)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_HAMBURGER_SIZE; ++i)
        _hamburger[i] = NULL;
}

TINY_BURGER static void __load_hamburger(int32_t level)
{
    switch (level)
    {
    case 0:
        __loadl_level0();
        break;
    case 1:
        __loadl_level1();
        break;
    case 2:
        __loadl_level2();
        break;
    case 3:
        __loadl_level3();
        break;
    case 4:
        __loadl_level4();
        break;
    case 5:
        __loadl_level5();
        break;
    }
}

TINY_BURGER static void __update_hamburger(void)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_HAMBURGER_SIZE; ++i)
    {
        if (_hamburger[i] != NULL)
        {
            update_hamburger(_hamburger[i], get_collision_shape_player(_player));
            if (!_isHamburgerCompletedByIndex[i] && is_completed_hamburger(_hamburger[i]))
            {
                _isHamburgerCompletedByIndex[i] = true;
                ++_countHamburger;
            }
        }
    }
}
TINY_BURGER static void __draw_hamburger(void)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_HAMBURGER_SIZE; ++i)
    {
        if (_hamburger[i] != NULL)
            draw_hamburger(_hamburger[i]);
    }
}

TINY_BURGER static void __unload_hamburger(void)
{
    for (uint32_t i = 0; i < TINY_BURGER_MAX_HAMBURGER_SIZE; ++i)
    {
        if (_hamburger[i] != NULL)
        {
            destroy_hamburger(&_hamburger[i]);
            _hamburger[i] = NULL;
        }
    }
}

TINY_BURGER static void __loadl_level0(void)
{
    _maxHamburger = 4;
    IngredientPath_t *path0 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path0->size = 6;
    path0->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path0->size);
    path0->vector[0] = (Vector2){1, -1};
    path0->vector[1] = (Vector2){1, 1};
    path0->vector[2] = (Vector2){1, 3};
    path0->vector[3] = (Vector2){1, 6};
    path0->vector[4] = (Vector2){1, 8};
    path0->vector[5] = (Vector2){1, 11};
    _hamburger[0] = create_hamburger((Rectangle){
                                         TINY_BURGER_TILE,
                                         TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         8 * TINY_BURGER_TILE},
                                     path0);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_UP, 1, 3);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_SALAD, 2, 2);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_MEAT, 3, 1);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_DOWN, 4, 0);

    IngredientPath_t *path1 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path1->size = 6;
    path1->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path1->size);
    path1->vector[0] = (Vector2){5, -1};
    path1->vector[1] = (Vector2){5, 2};
    path1->vector[2] = (Vector2){5, 4};
    path1->vector[3] = (Vector2){5, 6};
    path1->vector[4] = (Vector2){5, 8};
    path1->vector[5] = (Vector2){5, 11};
    _hamburger[1] = create_hamburger((Rectangle){
                                         5 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         10 * TINY_BURGER_TILE},
                                     path1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 3);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_SALAD, 2, 2);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_MEAT, 3, 1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_DOWN, 4, 0);

    IngredientPath_t *path2 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path2->size = 6;
    path2->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path2->size);
    path2->vector[0] = (Vector2){9, -1};
    path2->vector[1] = (Vector2){9, 1};
    path2->vector[2] = (Vector2){9, 4};
    path2->vector[3] = (Vector2){9, 6};
    path2->vector[4] = (Vector2){9, 8};
    path2->vector[5] = (Vector2){9, 11};
    _hamburger[2] = create_hamburger((Rectangle){
                                         9 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         10 * TINY_BURGER_TILE},
                                     path2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 3);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_SALAD, 1, 2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_MEAT, 3, 1);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_DOWN, 4, 0);

    IngredientPath_t *path3 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path3->size = 6;
    path3->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path3->size);
    path3->vector[0] = (Vector2){13, -1};
    path3->vector[1] = (Vector2){13, 1};
    path3->vector[2] = (Vector2){13, 3};
    path3->vector[3] = (Vector2){13, 5};
    path3->vector[4] = (Vector2){13, 8};
    path3->vector[5] = (Vector2){13, 11};
    _hamburger[3] = create_hamburger((Rectangle){
                                         13 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         10 * TINY_BURGER_TILE},
                                     path3);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 3);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_SALAD, 1, 2);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_MEAT, 2, 1);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_DOWN, 3, 0);
}

TINY_BURGER static void __loadl_level1(void)
{
    _maxHamburger = 4;
    IngredientPath_t *path0 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path0->size = 5;
    path0->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path0->size);
    path0->vector[0] = (Vector2){1, -1};
    path0->vector[1] = (Vector2){1, 0};
    path0->vector[2] = (Vector2){1, 1};
    path0->vector[3] = (Vector2){1, 3};
    path0->vector[4] = (Vector2){1, 8};
    _hamburger[0] = create_hamburger((Rectangle){
                                         TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         5 * TINY_BURGER_TILE},
                                     path0);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 3);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_SALAD, 1, 2);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_MEAT, 2, 1);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_DOWN, 3, 0);

    IngredientPath_t *path1 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path1->size = 7;
    path1->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path1->size);
    path1->vector[0] = (Vector2){5, -1};
    path1->vector[1] = (Vector2){5, 0};
    path1->vector[2] = (Vector2){5, 2};
    path1->vector[3] = (Vector2){5, 3};
    path1->vector[4] = (Vector2){5, 5};
    path1->vector[5] = (Vector2){5, 7};
    path1->vector[6] = (Vector2){5, 10};
    _hamburger[1] = create_hamburger((Rectangle){
                                         5 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         9 * TINY_BURGER_TILE},
                                     path1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 3);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_SALAD, 1, 2);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_MEAT, 2, 1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_DOWN, 5, 0);

    IngredientPath_t *path2 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path2->size = 7;
    path2->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path2->size);
    path2->vector[0] = (Vector2){9, -1};
    path2->vector[1] = (Vector2){9, 1};
    path2->vector[2] = (Vector2){9, 3};
    path2->vector[3] = (Vector2){9, 4};
    path2->vector[4] = (Vector2){9, 6};
    path2->vector[5] = (Vector2){9, 7};
    path2->vector[6] = (Vector2){9, 10};
    _hamburger[2] = create_hamburger((Rectangle){
                                         9 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         9 * TINY_BURGER_TILE},
                                     path2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 3);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_SALAD, 3, 2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_MEAT, 4, 1);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_DOWN, 5, 0);

    IngredientPath_t *path3 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path3->size = 5;
    path3->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path3->size);
    path3->vector[0] = (Vector2){13, -1};
    path3->vector[1] = (Vector2){13, 1};
    path3->vector[2] = (Vector2){13, 2};
    path3->vector[3] = (Vector2){13, 3};
    path3->vector[4] = (Vector2){13, 8};
    _hamburger[3] = create_hamburger((Rectangle){
                                         13 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         5 * TINY_BURGER_TILE},
                                     path3);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 3);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_SALAD, 1, 2);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_MEAT, 2, 1);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_DOWN, 3, 0);
}

TINY_BURGER static void __loadl_level2(void)
{
    _maxHamburger = 6;
    IngredientPath_t *path0 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path0->size = 5;
    path0->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path0->size);
    path0->vector[0] = (Vector2){1, -1};
    path0->vector[1] = (Vector2){1, 0};
    path0->vector[2] = (Vector2){1, 1};
    path0->vector[3] = (Vector2){1, 2};
    path0->vector[4] = (Vector2){1, 4};
    _hamburger[0] = create_hamburger((Rectangle){
                                         TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         4 * TINY_BURGER_TILE},
                                     path0);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 2);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_MEAT, 2, 1);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_DOWN, 3, 0);

    IngredientPath_t *path1 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path1->size = 6;
    path1->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path1->size);
    path1->vector[0] = (Vector2){5, -1};
    path1->vector[1] = (Vector2){5, 0};
    path1->vector[2] = (Vector2){5, 1};
    path1->vector[3] = (Vector2){5, 2};
    path1->vector[4] = (Vector2){5, 4};
    path1->vector[5] = (Vector2){5, 7};
    _hamburger[1] = create_hamburger((Rectangle){
                                         5 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         6 * TINY_BURGER_TILE},
                                     path1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 2);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_CHEESE, 1, 1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_DOWN, 3, 0);

    IngredientPath_t *path2 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path2->size = 6;
    path2->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path2->size);
    path2->vector[0] = (Vector2){9, -1};
    path2->vector[1] = (Vector2){9, 0};
    path2->vector[2] = (Vector2){9, 2};
    path2->vector[3] = (Vector2){9, 3};
    path2->vector[4] = (Vector2){9, 4};
    path2->vector[5] = (Vector2){9, 7};
    _hamburger[2] = create_hamburger((Rectangle){
                                         9 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         6 * TINY_BURGER_TILE},
                                     path2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_MEAT, 1, 1);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_DOWN, 2, 0);

    IngredientPath_t *path3 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path3->size = 5;
    path3->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path3->size);
    path3->vector[0] = (Vector2){13, -1};
    path3->vector[1] = (Vector2){13, 0};
    path3->vector[2] = (Vector2){13, 1};
    path3->vector[3] = (Vector2){13, 2};
    path3->vector[4] = (Vector2){13, 4};
    _hamburger[3] = create_hamburger((Rectangle){
                                         13 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         4 * TINY_BURGER_TILE},
                                     path3);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 2);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_CHEESE, 2, 1);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_DOWN, 3, 0);

    // ---
    IngredientPath_t *path4 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path4->size = 4;
    path4->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path4->size);
    path4->vector[0] = (Vector2){1, 6};
    path4->vector[1] = (Vector2){1, 7};
    path4->vector[2] = (Vector2){1, 8};
    path4->vector[3] = (Vector2){1, 10};
    _hamburger[4] = create_hamburger((Rectangle){
                                         TINY_BURGER_TILE,
                                         6 * TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE},
                                     path4);
    add_hamburger(_hamburger[4], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 2);
    add_hamburger(_hamburger[4], TINY_BURGER_INGREDIENT_CHEESE, 1, 1);
    add_hamburger(_hamburger[4], TINY_BURGER_INGREDIENT_BREAD_DOWN, 2, 0);

    IngredientPath_t *path5 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path5->size = 4;
    path5->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path5->size);
    path5->vector[0] = (Vector2){13, 6};
    path5->vector[1] = (Vector2){13, 7};
    path5->vector[2] = (Vector2){13, 8};
    path5->vector[3] = (Vector2){13, 10};
    _hamburger[5] = create_hamburger((Rectangle){
                                         13 * TINY_BURGER_TILE,
                                         6 * TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE},
                                     path5);
    add_hamburger(_hamburger[5], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 2);
    add_hamburger(_hamburger[5], TINY_BURGER_INGREDIENT_MEAT, 1, 1);
    add_hamburger(_hamburger[5], TINY_BURGER_INGREDIENT_BREAD_DOWN, 2, 0);
}

TINY_BURGER static void __loadl_level3(void)
{
    _maxHamburger = 4;
    IngredientPath_t *path0 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path0->size = 9;
    path0->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path0->size);
    path0->vector[0] = (Vector2){1, -1};
    path0->vector[1] = (Vector2){1, 0};
    path0->vector[2] = (Vector2){1, 1};
    path0->vector[3] = (Vector2){1, 2};
    path0->vector[4] = (Vector2){1, 3};
    path0->vector[5] = (Vector2){1, 4};
    path0->vector[6] = (Vector2){1, 5};
    path0->vector[7] = (Vector2){1, 6};
    path0->vector[8] = (Vector2){1, 11};
    _hamburger[0] = create_hamburger((Rectangle){
                                         TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         8 * TINY_BURGER_TILE},
                                     path0);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 7);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_TOMATO, 1, 6);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_CHEESE, 2, 5);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_SALAD, 3, 4);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_MEAT, 4, 3);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_SALAD, 5, 2);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_CHEESE, 6, 1);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_DOWN, 7, 0);

    IngredientPath_t *path1 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path1->size = 9;
    path1->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path1->size);
    path1->vector[0] = (Vector2){5, -1};
    path1->vector[1] = (Vector2){5, 0};
    path1->vector[2] = (Vector2){5, 1};
    path1->vector[3] = (Vector2){5, 2};
    path1->vector[4] = (Vector2){5, 3};
    path1->vector[5] = (Vector2){5, 4};
    path1->vector[6] = (Vector2){5, 5};
    path1->vector[7] = (Vector2){5, 6};
    path1->vector[8] = (Vector2){5, 11};
    _hamburger[1] = create_hamburger((Rectangle){
                                         5 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         8 * TINY_BURGER_TILE},
                                     path1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 7);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_TOMATO, 1, 6);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_CHEESE, 2, 5);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_SALAD, 3, 4);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_MEAT, 4, 3);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_SALAD, 5, 2);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_CHEESE, 6, 1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_DOWN, 7, 0);

    IngredientPath_t *path2 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path2->size = 9;
    path2->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path2->size);
    path2->vector[0] = (Vector2){9, -1};
    path2->vector[1] = (Vector2){9, 0};
    path2->vector[2] = (Vector2){9, 1};
    path2->vector[3] = (Vector2){9, 2};
    path2->vector[4] = (Vector2){9, 3};
    path2->vector[5] = (Vector2){9, 4};
    path2->vector[6] = (Vector2){9, 5};
    path2->vector[7] = (Vector2){9, 6};
    path2->vector[8] = (Vector2){9, 11};
    _hamburger[2] = create_hamburger((Rectangle){
                                         9 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         8 * TINY_BURGER_TILE},
                                     path2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 7);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_TOMATO, 1, 6);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_CHEESE, 2, 5);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_SALAD, 3, 4);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_MEAT, 4, 3);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_SALAD, 5, 2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_CHEESE, 6, 1);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_DOWN, 7, 0);

    IngredientPath_t *path3 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path3->size = 9;
    path3->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path3->size);
    path3->vector[0] = (Vector2){13, -1};
    path3->vector[1] = (Vector2){13, 0};
    path3->vector[2] = (Vector2){13, 1};
    path3->vector[3] = (Vector2){13, 2};
    path3->vector[4] = (Vector2){13, 3};
    path3->vector[5] = (Vector2){13, 4};
    path3->vector[6] = (Vector2){13, 5};
    path3->vector[7] = (Vector2){13, 6};
    path3->vector[8] = (Vector2){13, 11};
    _hamburger[3] = create_hamburger((Rectangle){
                                         13 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         8 * TINY_BURGER_TILE},
                                     path3);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 7);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_TOMATO, 1, 6);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_CHEESE, 2, 5);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_SALAD, 3, 4);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_MEAT, 4, 3);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_SALAD, 5, 2);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_CHEESE, 6, 1);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_DOWN, 7, 0);
}

TINY_BURGER static void __loadl_level4(void)
{
    _maxHamburger = 2;
    IngredientPath_t *path0 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path0->size = 9;
    path0->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path0->size);
    path0->vector[0] = (Vector2){5, -1};
    path0->vector[1] = (Vector2){5, 0};
    path0->vector[2] = (Vector2){5, 1};
    path0->vector[3] = (Vector2){5, 2};
    path0->vector[4] = (Vector2){5, 3};
    path0->vector[5] = (Vector2){5, 4};
    path0->vector[6] = (Vector2){5, 5};
    path0->vector[7] = (Vector2){5, 6};
    path0->vector[8] = (Vector2){5, 11};
    _hamburger[0] = create_hamburger((Rectangle){
                                         5 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         8 * TINY_BURGER_TILE},
                                     path0);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 7);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_CHEESE, 1, 6);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_TOMATO, 2, 5);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_SALAD, 3, 4);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_MEAT, 4, 3);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_SALAD, 5, 2);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_CHEESE, 6, 1);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_DOWN, 7, 0);

    IngredientPath_t *path1 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path1->size = 9;
    path1->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path1->size);
    path1->vector[0] = (Vector2){9, -1};
    path1->vector[1] = (Vector2){9, 0};
    path1->vector[2] = (Vector2){9, 1};
    path1->vector[3] = (Vector2){9, 2};
    path1->vector[4] = (Vector2){9, 3};
    path1->vector[5] = (Vector2){9, 4};
    path1->vector[6] = (Vector2){9, 5};
    path1->vector[7] = (Vector2){9, 6};
    path1->vector[8] = (Vector2){9, 11};
    _hamburger[1] = create_hamburger((Rectangle){
                                         9 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         8 * TINY_BURGER_TILE},
                                     path1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 7);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_CHEESE, 1, 6);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_TOMATO, 2, 5);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_SALAD, 3, 4);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_MEAT, 4, 3);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_SALAD, 5, 2);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_CHEESE, 6, 1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_DOWN, 7, 0);
}

TINY_BURGER static void __loadl_level5(void)
{
    _maxHamburger = 4;
    IngredientPath_t *path0 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path0->size = 6;
    path0->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path0->size);
    path0->vector[0] = (Vector2){1, 0};
    path0->vector[1] = (Vector2){1, 2};
    path0->vector[2] = (Vector2){1, 4};
    path0->vector[3] = (Vector2){1, 6};
    path0->vector[4] = (Vector2){1, 8};
    path0->vector[5] = (Vector2){1, 11};
    _hamburger[0] = create_hamburger((Rectangle){
                                         TINY_BURGER_TILE,
                                         0,
                                         3 * TINY_BURGER_TILE,
                                         9 * TINY_BURGER_TILE},
                                     path0);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 3);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_CHEESE, 1, 2);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_MEAT, 2, 1);
    add_hamburger(_hamburger[0], TINY_BURGER_INGREDIENT_BREAD_DOWN, 3, 0);

    IngredientPath_t *path1 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path1->size = 6;
    path1->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path1->size);
    path1->vector[0] = (Vector2){5, -1};
    path1->vector[1] = (Vector2){5, 1};
    path1->vector[2] = (Vector2){5, 3};
    path1->vector[3] = (Vector2){5, 5};
    path1->vector[4] = (Vector2){5, 7};
    path1->vector[5] = (Vector2){5, 11};
    _hamburger[1] = create_hamburger((Rectangle){
                                         5 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         9 * TINY_BURGER_TILE},
                                     path1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 4);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_CHEESE, 1, 3);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_MEAT, 2, 2);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_CHEESE, 3, 1);
    add_hamburger(_hamburger[1], TINY_BURGER_INGREDIENT_BREAD_DOWN, 4, 0);

    IngredientPath_t *path2 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path2->size = 6;
    path2->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path2->size);
    path2->vector[0] = (Vector2){9, 0};
    path2->vector[1] = (Vector2){9, 2};
    path2->vector[2] = (Vector2){9, 4};
    path2->vector[3] = (Vector2){9, 6};
    path2->vector[4] = (Vector2){9, 7};
    path2->vector[5] = (Vector2){9, 11};
    _hamburger[2] = create_hamburger((Rectangle){
                                         9 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         9 * TINY_BURGER_TILE},
                                     path2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 4);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_TOMATO, 1, 3);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_CHEESE, 2, 2);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_MEAT, 3, 1);
    add_hamburger(_hamburger[2], TINY_BURGER_INGREDIENT_BREAD_DOWN, 4, 0);

    IngredientPath_t *path3 = (IngredientPath_t *)MemAlloc(sizeof(IngredientPath_t));
    path3->size = 6;
    path3->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * path3->size);
    path3->vector[0] = (Vector2){13, -1};
    path3->vector[1] = (Vector2){13, 1};
    path3->vector[2] = (Vector2){13, 3};
    path3->vector[3] = (Vector2){13, 5};
    path3->vector[4] = (Vector2){13, 7};
    path3->vector[5] = (Vector2){13, 11};
    _hamburger[3] = create_hamburger((Rectangle){
                                         13 * TINY_BURGER_TILE,
                                         -TINY_BURGER_TILE,
                                         3 * TINY_BURGER_TILE,
                                         9 * TINY_BURGER_TILE},
                                     path3);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_UP, 0, 3);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_MEAT, 1, 2);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_CHEESE, 3, 1);
    add_hamburger(_hamburger[3], TINY_BURGER_INGREDIENT_BREAD_DOWN, 4, 0);
}