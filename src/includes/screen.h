#ifndef __TINY_BURGER_SCREEN_H__
#define __TINY_BURGER_SCREEN_H__

#include "config.h"

typedef struct
{
    void (*update_screen)(void);
    void (*draw_screen)(void);
} Screen_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Screen_t *create_menu(void);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_SCREEN_H__