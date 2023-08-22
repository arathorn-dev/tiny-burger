#ifndef __TINY_BURGER_SCREEN_H__
#define __TINY_BURGER_SCREEN_H__

#include "config.h"

typedef enum
{
    SCREEN_TYPE_EMPTY = -1,
    SCREEN_TYPE_MENU = 0,
    SCREEN_TYPE_GAME,

} ScreenType_u;

typedef struct
{
    ScreenType_u currentScreenType;
    ScreenType_u nextScreenType;
} Screen_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Screen_t *create_menu(void);
    TINY_BURGER void update_menu(Screen_t *const);
    TINY_BURGER void draw_menu(const Screen_t *const);
    TINY_BURGER void destroy_menu(Screen_t **);

    TINY_BURGER Screen_t *create_game(void);
    TINY_BURGER void update_game(Screen_t *const);
    TINY_BURGER void draw_game(const Screen_t *const);
    TINY_BURGER void destroy_game(Screen_t **);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_SCREEN_H__