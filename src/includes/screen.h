#ifndef __TINY_BURGER_SCREEN_H__
#define __TINY_BURGER_SCREEN_H__

#include "config.h"

TINY_BURGER typedef enum {
    TB_SCREEN_TYPE_EMPTY = -1,
    TB_SCREEN_TYPE_EXIT = 0,
    TB_SCREEN_TYPE_MENU,
    TB_SCREEN_TYPE_GAME,
    TB_SCREEN_TYPE_OPTION,

} ScreenType_u;

TINY_BURGER typedef struct
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

    TINY_BURGER Screen_t *create_game(const char *);
    TINY_BURGER void update_game(Screen_t *const);
    TINY_BURGER void draw_game(const Screen_t *const);
    TINY_BURGER void destroy_game(Screen_t **);

    TINY_BURGER Screen_t *create_option(void);
    TINY_BURGER void update_option(Screen_t *const);
    TINY_BURGER void draw_option(const Screen_t *const);
    TINY_BURGER void destroy_option(Screen_t **);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_SCREEN_H__