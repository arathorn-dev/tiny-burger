#include "includes/utils.h"

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

TINY_BURGER static int32_t *__create_vector(const char *, size_t, size_t);
TINY_BURGER static char **__copy_text(const char **, int32_t);
TINY_BURGER static void __destroy_copy_text(char ***, int32_t);

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------

TINY_BURGER int32_t *load_map(const char *fileName, size_t width, size_t height)
{
    int32_t *vector = NULL;
    if (FileExists(fileName))
    {
        TraceLog(LOG_INFO, TextFormat("the file: %s exists", fileName));
        vector = __create_vector(fileName, width, height);
    }
    else
    {
        TraceLog(LOG_INFO, TextFormat("Error to load the file: %s", fileName));
    }

    return vector;
}

TINY_BURGER void unload_map(int32_t **ptr)
{
    if ((*ptr) != NULL)
    {
        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_INFO, "The map has been deleted");
    }
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static int32_t *__create_vector(const char *fileName, size_t width, size_t height)
{
    char *data = LoadFileText(fileName);
    int32_t *vector = MemAlloc(sizeof(int32_t) * width * height);
    if (data != NULL)
    {
        int32_t rowsCount = 0;
        const char **tmpRowsText = TextSplit(data, '\n', &rowsCount);
        char **rowsText = __copy_text(tmpRowsText, rowsCount);

        for (size_t i = 0; i < rowsCount; ++i)
        {
            int32_t columnCount = 0;
            const char **columnsText = TextSplit(rowsText[i], ',', &columnCount);
            for (size_t j = 0; j < columnCount; ++j)
            {
                int32_t value = -1;
                if (!TextIsEqual(columnsText[j], "-1"))
                {
                    value = TextToInteger(columnsText[j]);
                }

                vector[j + i * width] = value;
            }
        }

        __destroy_copy_text(&rowsText, rowsCount);
        UnloadFileText(data);
        TraceLog(LOG_INFO, "File has been closed successfully.");
    }
    return vector;
}

TINY_BURGER static char **__copy_text(const char **text, int32_t size)
{
    char **newText = (char **)MemAlloc(sizeof(const char *) * size);

    for (size_t i = 0; i < size; ++i)
    {
        size_t s = TextLength(text[i]) + 1;
        newText[i] = MemAlloc(sizeof(char) * s);
        TextCopy(newText[i], text[i]);
    }

    return newText;
}

TINY_BURGER static void __destroy_copy_text(char ***ptr, int32_t size)
{
    if ((*ptr) != NULL)
    {
        for (size_t i = 0; i < size; ++i)
        {
            MemFree((*ptr)[i]);
            (*ptr)[i] = NULL;
        }
        MemFree((*ptr));
        (*ptr) = NULL;
    }
}