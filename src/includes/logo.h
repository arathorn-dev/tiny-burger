#ifndef __TINY_BURGER_LOGO_H__
#define __TINY_BURGER_LOGO_H__

#include "config.h"

TINY_BURGER typedef struct
{
    Rectangle source;
    Rectangle dest;
} Logo_t;

#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER Logo_t *create_logo(void);
    TINY_BURGER void draw_logo(const Logo_t *const logo);
    TINY_BURGER void destroy_logo(Logo_t **ptr);
#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_LOGO_H__