#ifndef __TINY_BURGER_GUI_H__
#define __TINY_BURGER_GUI_H__

#include "config.h"

TINY_BURGER typedef struct
{
    uint32_t bulletAmount;
    uint32_t lives;
    uint32_t currentPoints;
    uint32_t maxPoints;
    uint32_t levelCounter;
} GuiData_t;

TINY_BURGER typedef struct
{
    Vector2 position;
    bool hasImage;
    union
    {
        char *label;
        Rectangle *imageRect;
    };

} GuiInfo_t;

TINY_BURGER typedef struct
{
    Color color;
    Color labelColor;
    GuiInfo_t info[TINY_BURGER_INFO_GUI_SIZE];
} Gui_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER GuiData_t *create_gui_data(void);
    TINY_BURGER void destroy_gui_data(GuiData_t **ptr);

    TINY_BURGER Gui_t *create_gui(void);
    TINY_BURGER void update_gui(Gui_t *const gui);
    TINY_BURGER void draw_gui(const Gui_t *const gui);
    TINY_BURGER void destroy_gui(Gui_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_GUI_H__