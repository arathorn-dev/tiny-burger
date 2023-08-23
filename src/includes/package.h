#ifndef __TINY_BURGER_PACKAGE_H__
#define __TINY_BURGER_PACKAGE_H__

#include "config.h"

TINY_BURGER typedef enum {
    TB_FONT_TYPE_04B03 = 0
} FontType_e;

TINY_BURGER typedef enum {
    TB_TEXTURE_TYPE_TILE = 0
} TextureType_e;

TINY_BURGER typedef struct
{
    Font fonts[TINY_BURGER_FONT_COUNT];
    Texture2D textures[TINY_BURGER_TEXTURE_COUNT];
} Package_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER Package_t *create_package(void);
    TINY_BURGER void destroy_package(Package_t **);

#if defined(__cplusplus)
}
#endif

#endif //__TINY_BURGER_PACKAGE_H__
