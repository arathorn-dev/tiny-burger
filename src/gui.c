#include "includes/gui.h"
#include "includes/package.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------
extern Package_t *globalPackage;

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
TINY_BURGER static Image *_image;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER static void __init_info(Gui_t *const gui);
    TINY_BURGER static void __update_info(Gui_t *const gui);
    TINY_BURGER static void __draw_info(const Gui_t *const gui);
    TINY_BURGER static void __destroy_info(Gui_t *const gui);

#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER Gui_t *create_gui(void)
{
    Gui_t *gui = MemAlloc(sizeof(Gui_t));
    if (gui == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Gui_t pointer.");
        return NULL;
    }

    gui->transform = (Rectangle){
        0,
        0,
        TINY_BURGER_WIDTH,
        TINY_BURGER_TILE * 2,
    };
    gui->color = GetColor(TINY_BURGER_COLOR_0);
    gui->pepper = 0;
    gui->lives = 0;
    gui->currentPoints = 0;
    gui->maxPoints = 0;

    __init_info(gui);

    return gui;
}
TINY_BURGER void update_gui(Gui_t *const gui)
{
    __update_info(gui);
}
TINY_BURGER void draw_gui(const Gui_t *const gui)
{
    __draw_info(gui);
}
TINY_BURGER void destroy_gui(Gui_t **ptr)
{
    if ((*ptr) != NULL)
    {
        __destroy_info((*ptr));
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Gui_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __init_info(Gui_t *const gui)
{
    uint32_t size = 12;
    // ----
    gui->info[0].position = (Vector2){0, 0};
    gui->info[0].hasImage = false;
    gui->info[0].text = MemAlloc(sizeof(char) * size);
    TextCopy(gui->info[0].text, "000000");
    gui->info[0].label = MemAlloc(sizeof(char) * size);
    TextCopy(gui->info[0].label, "1UP");

    // ----
    gui->info[1].position = (Vector2){0, 0};
    gui->info[1].hasImage = false;
    gui->info[1].text = MemAlloc(sizeof(char) * size);
    TextCopy(gui->info[1].text, "000000");
    gui->info[1].label = MemAlloc(sizeof(char) * size);
    TextCopy(gui->info[1].label, "HI");

    // ----
    gui->info[2].position = (Vector2){0, 0};
    gui->info[2].hasImage = true;
    gui->info[2].text = MemAlloc(sizeof(char) * size);
    TextCopy(gui->info[2].text, "2");
    gui->info[2].imageRect = MemAlloc(sizeof(Rectangle));
    gui->info[2].imageRect->x = TINY_BURGER_TILE * 6;
    gui->info[2].imageRect->y = 0;
    gui->info[2].imageRect->width = 8;
    gui->info[2].imageRect->height = 9;

    // ----
    gui->info[3].position = (Vector2){0, 0};
    gui->info[3].hasImage = true;
    gui->info[3].text = MemAlloc(sizeof(char) * size);
    TextCopy(gui->info[3].text, "2");
    gui->info[3].imageRect = MemAlloc(sizeof(Rectangle));
    gui->info[3].imageRect->x = TINY_BURGER_TILE * 6 + 8;
    gui->info[3].imageRect->y = 0;
    gui->info[3].imageRect->width = 8;
    gui->info[3].imageRect->height = 9;
}

TINY_BURGER static void __update_info(Gui_t *const gui)
{
    // TODO
}
TINY_BURGER static void __draw_info(const Gui_t *const gui)
{
    Vector2 position = (Vector2){54, 8};
    for (uint32_t i = 0; i < TINY_BURGER_INFO_GUI_SIZE; ++i)
    {
        if (gui->info[i].hasImage)
        {
            DrawTexturePro(
                globalPackage->textures[TB_TEXTURE_TYPE_TILE],
                *(gui->info[i].imageRect),
                (Rectangle){
                    position.x,
                    position.y,
                    24,
                    24},
                (Vector2){0},
                0,
                RAYWHITE);
        }
        else
        {
            DrawTextEx(
                globalPackage->fonts[TB_FONT_TYPE_04B03],
                gui->info[i].label,
                position,
                24,
                1,
                RAYWHITE);
        }
        position.x += (i > 1) ? 32 : 64;
        DrawTextEx(
            globalPackage->fonts[TB_FONT_TYPE_04B03],
            gui->info[i].text,
            position,
            24,
            1,
            RAYWHITE);
        position.x += (i > 1) ? 64 : 128;
    }
}

TINY_BURGER static void __destroy_info(Gui_t *const gui)
{
    for (uint32_t i = 0; i < TINY_BURGER_INFO_GUI_SIZE; ++i)
    {
        if (gui->info[i].hasImage)
        {
            MemFree(gui->info[i].imageRect);
            gui->info[i].imageRect = NULL;
        }
        else
        {
            MemFree(gui->info[i].label);
            gui->info[i].label = NULL;
        }

        MemFree(gui->info[i].text);
        gui->info[i].text = NULL;
    }
}