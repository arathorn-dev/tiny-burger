#include "includes/app.h"

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

TINY_BURGER static void __init_window(void);
TINY_BURGER static void __close_window(void);
TINY_BURGER static void __destroy_app(App_t **);
TINY_BURGER static void __update_window(App_t *);
TINY_BURGER static void __draw_window(App_t *);

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER App_t *create_app(void)
{
    App_t *app = MemAlloc(sizeof(App_t));
    if (app == NULL)
    {
        // TODO
    }
    app->screen = NULL;
    __init_window();
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
    InitWindow(TINY_BURGER_WIDTH, TINY_BURGER_HEIGHT, TINY_BURGER_TITLE);
    SetTargetFPS(TINY_BURGER_FPS);
}

TINY_BURGER static void __close_window(void)
{
    CloseWindow();
}

TINY_BURGER static void __destroy_app(App_t **ptr)
{
    if ((*ptr) != NULL)
    {
        MemFree((*ptr));
        (*ptr) = NULL;
    }
}

TINY_BURGER static void __update_window(App_t *app)
{
    // TODO
}
TINY_BURGER static void __draw_window(App_t *app)
{
    ClearBackground(GetColor(TINY_BURGER_COLOR_10));
    BeginDrawing();
    // TODO
    EndDrawing();
}