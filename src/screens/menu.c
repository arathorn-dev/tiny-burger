#include "../includes/screen.h"
#include "../includes/package.h"
#include "../includes/pointer.h"
#include "../includes/logo.h"

#define __MENU_SIZE 3

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

static const char *_options[__MENU_SIZE] = {
    "PLAYER",
    "OPTION",
    "EXIT"};
static Vector2 _optionsPosition[__MENU_SIZE];
static int32_t currentOption = 0;
static Pointer_t *pointer = NULL;
static Logo_t *logo = NULL;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER static void __load_options(void);
    TINY_BURGER static void __update_options(Screen_t *const screen);
    TINY_BURGER static void __draw_options(void);
    TINY_BURGER static void __select_option(Screen_t *const screen);

#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------

TINY_BURGER Screen_t *create_menu(void)
{
    Screen_t *screen = (Screen_t *)MemAlloc(sizeof(Screen_t));
    if (screen == NULL)
    {
        TraceLog(LOG_DEBUG, "[MENU] Error to create a Screen_t pointer.");
        return NULL;
    }

    screen->currentScreenType = TB_SCREEN_TYPE_MENU;
    screen->nextScreenType = TB_SCREEN_TYPE_EMPTY;
    __load_options();

    pointer = create_pointer(_optionsPosition[currentOption]);
    if (!pointer)
    {
        MemFree(screen);
        screen = NULL;
        return NULL;
    }

    logo = create_logo();
    if (!logo)
    {
        destroy_pointer(&pointer);
        MemFree(screen);
        screen = NULL;
        return NULL;
    }
    TraceLog(LOG_DEBUG, "[MENU] Screen_t pointer created successfully.");

    return screen;
}

TINY_BURGER void update_menu(Screen_t *const screen)
{
    __update_options(screen);
    update_pointer(pointer, _optionsPosition[currentOption]);
}
TINY_BURGER void draw_menu(const Screen_t *const screen)
{
    DrawRectangle(0, 0, TINY_BURGER_WIDTH, TINY_BURGER_HEIGHT, GetColor(TINY_BURGER_COLOR_0));
    draw_logo(logo);
    __draw_options();
}

TINY_BURGER void destroy_menu(Screen_t **ptr)
{
    if ((*ptr) != NULL)
    {
        destroy_logo(&logo);
        destroy_pointer(&pointer);
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "[MENU] Screen_t pointer destroyed successfully.");
    }
}
//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __load_options(void)
{
    int32_t middle = TINY_BURGER_WIDTH / 2;
    Vector2 position = (Vector2){0, TINY_BURGER_HEIGHT - TINY_BURGER_FONT_SIZE * 8};
    for (size_t i = 0; i < __MENU_SIZE; ++i)
    {
        Vector2 measure = MeasureTextEx(globalPackage->fonts[TB_FONT_TYPE_04B03], _options[i], TINY_BURGER_FONT_SIZE, 1.0);
        _optionsPosition[i] =
            (Vector2){middle - (measure.x / 2), position.y + i * 32};
    }
}

TINY_BURGER static void __update_options(Screen_t *const screen)
{
    if (IsKeyPressed(KEY_UP))
    {
        currentOption = (currentOption - 1) % __MENU_SIZE;
        if (currentOption < 0)
            currentOption = __MENU_SIZE + currentOption;
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        currentOption = (currentOption + 1) % __MENU_SIZE;
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        __select_option(screen);
    }
}
TINY_BURGER static void __draw_options(void)
{
    Color normalColor = GetColor(TINY_BURGER_COLOR_15);
    Color selectedColor = GetColor(TINY_BURGER_COLOR_13);
    for (size_t i = 0; i < __MENU_SIZE; ++i)
    {
        DrawTextEx(
            globalPackage->fonts[TB_FONT_TYPE_04B03],
            _options[i],
            _optionsPosition[i],
            TINY_BURGER_FONT_SIZE,
            1.0,
            currentOption == i ? selectedColor : normalColor);
    }
    draw_pointer(pointer);
}

TINY_BURGER static void __select_option(Screen_t *const screen)
{
    switch (currentOption)
    {
    case 0:
        screen->nextScreenType = TB_SCREEN_TYPE_GAME;
        break;
    case 1:
        screen->nextScreenType = TB_SCREEN_TYPE_OPTION;
        break;
    case 2:
        screen->nextScreenType = TB_SCREEN_TYPE_EXIT;
        break;
    default:
        break;
    }
}