#ifndef __TINY_BURGER_APP_H__
#define __TINY_BURGER_APP_H__

#include "config.h"
#include "screen.h"

TINY_BURGER typedef struct
{
    Screen_t *screen;
} App_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER App_t *create_app(void);
    TINY_BURGER void run_app(App_t *);
    TINY_BURGER void destroy_app(App_t **);

#if defined(__cplusplus)
}
#endif

#endif //__TINY_BURGER_APP_H__