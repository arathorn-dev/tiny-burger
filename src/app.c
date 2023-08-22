#include "includes/app.h"

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

TINY_BURGER static void __init_window(void);
TINY_BURGER static void __close_window(void);
TINY_BURGER static void __destroy_app(App_t **);
TINY_BURGER static void __update_window(App_t *);
TINY_BURGER static void __draw_window(App_t *);

TINY_BURGER static void __update_screen(Screen_t *const);
TINY_BURGER static void __draw_screen(const Screen_t *const);
TINY_BURGER static void __change_screen(App_t *);
TINY_BURGER static void __change_screen_to(App_t *, ScreenType_u);
TINY_BURGER static Screen_t *__load_screen(ScreenType_u);
TINY_BURGER static void __unload_screen(Screen_t *);

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
    app->screen = __load_screen(SCREEN_TYPE_MENU);
    if (app->screen == NULL)
    {
        MemFree(app);
        app = NULL;
        return NULL;
    }
    __init_window();
    TraceLog(LOG_DEBUG, "App_t pointer created successfully.");
    return app;
}

TINY_BURGER void run_app(App_t *app)
{
    while (!WindowShouldClose())
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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
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

TINY_BURGER static void __destroy_app(App_t **ptr)
{
    if ((*ptr) != NULL)
    {
        __unload_screen((*ptr)->screen);
        TraceLog(LOG_DEBUG, "App_t pointer destroyed successfully.");
        MemFree((*ptr));
        (*ptr) = NULL;
    }
}

TINY_BURGER static void __update_window(App_t *app)
{
    if (IsKeyPressed(KEY_F1))
    {
        __change_screen_to(app, SCREEN_TYPE_MENU);
    }
    else if (IsKeyPressed(KEY_F2))
    {
        __change_screen_to(app, SCREEN_TYPE_GAME);
    }

    __update_screen(app->screen);
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
    case SCREEN_TYPE_MENU:
        update_menu(screen);
        break;

    case SCREEN_TYPE_GAME:
        update_game(screen);
        break;

    default:
        break;
    }
}

TINY_BURGER static void __draw_screen(const Screen_t *const screen)
{
    switch (screen->currentScreenType)
    {
    case SCREEN_TYPE_MENU:
        draw_menu(screen);
        break;

    case SCREEN_TYPE_GAME:
        draw_game(screen);
        break;

    default:
        break;
    }
}

TINY_BURGER static Screen_t *__load_screen(ScreenType_u type)
{
    Screen_t *screen = NULL;
    switch (type)
    {
    case SCREEN_TYPE_MENU:
        screen = create_menu();
        break;

    case SCREEN_TYPE_GAME:
        screen = create_game();
        break;

    default:
        break;
    }

    return screen;
}
TINY_BURGER static void __unload_screen(Screen_t *screen)
{
    if (screen != NULL)
    {
        switch (screen->currentScreenType)
        {
        case SCREEN_TYPE_MENU:
            destroy_menu(&screen);
            break;

        case SCREEN_TYPE_GAME:
            destroy_game(&screen);
            break;

        default:
            break;
        }
    }
}

TINY_BURGER static void __change_screen(App_t *app)
{
    if (app->screen != NULL)
    {
        ScreenType_u next = app->screen->nextScreenType;
        __unload_screen(app->screen);
        app->screen = __load_screen(next);
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