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
    if (!__load_fonts(package))
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
    if (FileExists("data/fonts/04B_03__.TTF"))
    {
        package->fonts[TB_FONT_TYPE_04B03] = LoadFont("data/fonts/04B_03__.TTF");
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