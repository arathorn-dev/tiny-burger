#include "includes/package.h"

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER static bool __load_fonts(Package_t *);
    TINY_BURGER static void __unload_fonts(Package_t *);

    TINY_BURGER static bool __load_textures(Package_t *);
    TINY_BURGER static void __unload_textures(Package_t *);

#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------

TINY_BURGER Package_t *create_package(void)
{
    Package_t *package = (Package_t *)MemAlloc(sizeof(Package_t));
    if (package == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create Package_t pointer.");
        return NULL;
    }

    bool isValid = __load_fonts(package) && __load_textures(package);
    if (!isValid)
    {
        MemFree(package);
        package = NULL;
        return package;
    }

    TraceLog(LOG_DEBUG, "Package_t pointer created successfully.");
    return package;
}
TINY_BURGER void destroy_package(Package_t **ptr)
{
    if ((*ptr) != NULL)
    {
        __unload_fonts((*ptr));
        __unload_textures((*ptr));
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "Package_t pointer destroyed successfully.");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static bool __load_fonts(Package_t *package)
{
    bool fontsLoaded = false;
    if (FileExists(TINY_BURGER_FONT_PATH_04B03))
    {
        package->fonts[TB_FONT_TYPE_04B03] = LoadFont(TINY_BURGER_FONT_PATH_04B03);
        fontsLoaded = true;
    }
    else
    {
        TraceLog(LOG_DEBUG, "[TB_FONT_TYPE_04B03] Error to load the font.");
    }

    return fontsLoaded;
}

TINY_BURGER static void __unload_fonts(Package_t *package)
{
    UnloadFont(package->fonts[TB_FONT_TYPE_04B03]);
    TraceLog(LOG_DEBUG, "[TB_FONT_TYPE_04B03] Font unloaded successfully.");
}

TINY_BURGER static bool __load_textures(Package_t *package)
{
    bool texturesLoaded = false;
    if (FileExists(TINY_BURGER_TEXTURE_PATH_TILE))
    {
        package->textures[TB_TEXTURE_TYPE_TILE] = LoadTexture(TINY_BURGER_TEXTURE_PATH_TILE);
        texturesLoaded = true;
    }

    return texturesLoaded;
}
TINY_BURGER static void __unload_textures(Package_t *package)
{
    UnloadTexture(package->textures[TB_TEXTURE_TYPE_TILE]);
    TraceLog(LOG_DEBUG, "[TB_TEXTURE_TYPE_TILE] Texture unloaded successfully.");
}