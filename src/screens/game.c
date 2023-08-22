#include "../includes/screen.h"

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

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

    screen->currentScreenType = TB_SCREEN_TYPE_GAME;
    screen->nextScreenType = TB_SCREEN_TYPE_EMPTY;

    TraceLog(LOG_DEBUG, "[GAME] Screen_t pointer created successfully.");
    return screen;
}

TINY_BURGER void update_game(Screen_t *const screen)
{
    // TODO
}
TINY_BURGER void draw_game(const Screen_t *const screen)
{
    DrawRectangle(0, 0, TINY_BURGER_WIDTH, TINY_BURGER_HEIGHT, GetColor(TINY_BURGER_COLOR_3));
}

TINY_BURGER void destroy_game(Screen_t **ptr)
{
    if ((*ptr) != NULL)
    {
        TraceLog(LOG_DEBUG, "[GAME] Screen_t pointer destroyed successfully.");
        MemFree((*ptr));
        (*ptr) = NULL;
    }
}
//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
