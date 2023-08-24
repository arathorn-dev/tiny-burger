#include "includes/app.h"
#include "includes/package.h"

//----------------------------------------------------------------------------------
// Global Definition.
//----------------------------------------------------------------------------------
Package_t *globalPackage = NULL;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER static void __init_window(void);
    TINY_BURGER static void __close_window(void);
    TINY_BURGER static void __destroy_app(App_t **ptr);
    TINY_BURGER static void __destroy_app_pointer(App_t *app);
    TINY_BURGER static void __update_window(App_t *app);
    TINY_BURGER static void __draw_window(App_t *app);

    TINY_BURGER static void __update_screen(Screen_t *const screen);
    TINY_BURGER static void __draw_screen(const Screen_t *const screen);
    TINY_BURGER static void __change_screen_to(App_t *app, ScreenType_u next);
    TINY_BURGER static Screen_t *__load_screen(ScreenType_u type);
    TINY_BURGER static void __unload_screen(Screen_t *);

    TINY_BURGER static void __transition_screen(Screen_t *screen, ScreenType_u next);
    TINY_BURGER static void __update_transition(App_t *app);
    TINY_BURGER static void __draw_transition(void);

#if defined(__cplusplus)
}
#endif

TINY_BURGER static float _transAlpha = 0.0f;
TINY_BURGER static bool _onTransition = false;
TINY_BURGER static bool _transFadeOut = false;
TINY_BURGER static ScreenType_u _currentScreen = TB_SCREEN_TYPE_EMPTY;
TINY_BURGER static ScreenType_u _nextScreen = TB_SCREEN_TYPE_EMPTY;

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER App_t *create_app(void)
{
    App_t *app = MemAlloc(sizeof(App_t));
    if (app == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create App_t pointer.");
        return NULL;
    }

    __init_window();
    globalPackage = create_package();
    if (globalPackage == NULL)
    {
        __close_window();
        __destroy_app_pointer(app);
        return NULL;
    }

    app->screen = __load_screen(TB_SCREEN_TYPE_MENU);
    if (app->screen == NULL)
    {
        __close_window();
        destroy_package(&globalPackage);
        globalPackage = NULL;
        __destroy_app_pointer(app);
        return NULL;
    }

    app->isRunning = true;
    TraceLog(LOG_DEBUG, "App_t pointer created successfully.");
    return app;
}

TINY_BURGER void run_app(App_t *app)
{
    while (app->isRunning)
    {
        __update_window(app);
        __draw_window(app);
    }
}

TINY_BURGER void destroy_app(App_t **ptr)
{
    __close_window();
    __destroy_app(ptr);
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __init_window(void)
{
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(TINY_BURGER_WIDTH, TINY_BURGER_HEIGHT, TINY_BURGER_TITLE);
    SetTargetFPS(TINY_BURGER_FPS);

#if defined(TINY_BURGER_DEBUG)
    SetTraceLogLevel(LOG_DEBUG);
#endif
}

TINY_BURGER static void __close_window(void)
{
    CloseWindow();
}

TINY_BURGER static void __destroy_app_pointer(App_t *app)
{
    MemFree(app);
    app = NULL;
    TraceLog(LOG_DEBUG, "App_t pointer destroyed successfully.");
}

TINY_BURGER static void __destroy_app(App_t **ptr)
{
    if ((*ptr) != NULL)
    {
        __unload_screen((*ptr)->screen);
        __destroy_app_pointer((*ptr));
    }
}

TINY_BURGER static void __update_window(App_t *app)
{
    // TODO: Improve this code.
    if (!_onTransition)
    {
        if (app->screen->nextScreenType != TB_SCREEN_TYPE_EMPTY)
        {
            if (app->screen->nextScreenType == TB_SCREEN_TYPE_EXIT)
                app->isRunning = false;
            else
                __transition_screen(app->screen, app->screen->nextScreenType);
        }

        __update_screen(app->screen);
    }
    else
    {
        __update_transition(app);
    }
}

TINY_BURGER static void __draw_window(App_t *app)
{
    ClearBackground(GetColor(TINY_BURGER_COLOR_0));
    BeginDrawing();
    __draw_screen(app->screen);
    EndDrawing();
}

TINY_BURGER static void __update_screen(Screen_t *const screen)
{
    switch (screen->currentScreenType)
    {
    case TB_SCREEN_TYPE_MENU:
        update_menu(screen);
        break;

    case TB_SCREEN_TYPE_GAME:
        update_game(screen);
        break;

    case TB_SCREEN_TYPE_OPTION:
        update_option(screen);
        break;

    default:
        break;
    }
}

TINY_BURGER static void __draw_screen(const Screen_t *const screen)
{
    switch (screen->currentScreenType)
    {
    case TB_SCREEN_TYPE_MENU:
        draw_menu(screen);
        break;

    case TB_SCREEN_TYPE_GAME:
        draw_game(screen);
        break;

    case TB_SCREEN_TYPE_OPTION:
        draw_option(screen);
        break;

    default:
        break;
    }

    if (_onTransition)
    {
        __draw_transition();
    }
}

TINY_BURGER static Screen_t *__load_screen(ScreenType_u type)
{
    Screen_t *screen = NULL;
    switch (type)
    {
    case TB_SCREEN_TYPE_MENU:
        screen = create_menu();
        break;

    case TB_SCREEN_TYPE_GAME:
        screen = create_game();
        break;

    case TB_SCREEN_TYPE_OPTION:
        screen = create_option();
        break;

    default:
        break;
    }
    _currentScreen = type;
    return screen;
}
TINY_BURGER static void __unload_screen(Screen_t *screen)
{
    if (screen != NULL)
    {
        switch (screen->currentScreenType)
        {
        case TB_SCREEN_TYPE_MENU:
            destroy_menu(&screen);
            break;

        case TB_SCREEN_TYPE_GAME:
            destroy_game(&screen);
            break;

        case TB_SCREEN_TYPE_OPTION:
            destroy_option(&screen);
            break;

        default:
            break;
        }
    }
}

TINY_BURGER static void __change_screen_to(App_t *app, ScreenType_u next)
{
    if (app->screen != NULL && app->screen->currentScreenType != next)
    {
        __unload_screen(app->screen);
        app->screen = __load_screen(next);
    }
}

TINY_BURGER static void __transition_screen(Screen_t *screen, ScreenType_u next)
{
    _onTransition = true;
    _transFadeOut = false;
    _nextScreen = next;
    _transAlpha = 0.0f;
}
TINY_BURGER static void __update_transition(App_t *app)
{
    if (!_transFadeOut)
    {
        _transAlpha += 0.09f;

        if (_transAlpha > 1.01f)
        {
            _transAlpha = 1.0f;
            __change_screen_to(app, _nextScreen);
            _transFadeOut = true;
        }
    }
    else
    {
        _transAlpha -= 0.09f;

        if (_transAlpha < -0.01f)
        {
            _transAlpha = 0.0f;
            _transFadeOut = false;
            _onTransition = false;
            _nextScreen = TB_SCREEN_TYPE_EMPTY;
        }
    }
}
TINY_BURGER static void __draw_transition(void)
{
    DrawRectangle(0, 0, TINY_BURGER_WIDTH, TINY_BURGER_HEIGHT, Fade(BLACK, _transAlpha));
}