#include "../includes/screen.h"

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

TINY_BURGER Screen_t *create_option(void)
{
    Screen_t *screen = (Screen_t *)MemAlloc(sizeof(Screen_t *));
    if (screen == NULL)
    {
        TraceLog(LOG_DEBUG, "[OPTION] Error to create a Screen_t pointer.");
        return NULL;
    }

    screen->currentScreenType = TB_SCREEN_TYPE_OPTION;
    screen->nextScreenType = TB_SCREEN_TYPE_EMPTY;

    TraceLog(LOG_DEBUG, "[OPTION] Screen_t pointer created successfully.");
    return screen;
}
TINY_BURGER void update_option(Screen_t *const screen)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        screen->nextScreenType = TB_SCREEN_TYPE_MENU;
    }
}
TINY_BURGER void draw_option(const Screen_t *const screen)
{
    DrawRectangle(0, 0, TINY_BURGER_WIDTH, TINY_BURGER_HEIGHT, GetColor(TINY_BURGER_COLOR_2));
}
TINY_BURGER void destroy_option(Screen_t **ptr)
{
    if ((*ptr) != NULL)
    {
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "[OPTION] Screen_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
