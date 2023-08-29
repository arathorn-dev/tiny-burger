#include "includes/pathfinding.h"

#define TB_PATH_SIZE 10

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------
TINY_BURGER Path_t *openList[TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT];
TINY_BURGER Path_t *closeList[TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT];

#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER static void __init_list(void);
    TINY_BURGER static void __evaluate_path(const int32_t *const map, Path_t *path, Vector2 end);
    TINY_BURGER static void __push_path_to_list(Path_t **list, Path_t *path);
    TINY_BURGER static void __remove_path_to_list(Path_t **list, Path_t *path);
    TINY_BURGER static Path_t *__get_path_open_list(void);
    TINY_BURGER static bool __exists_value_list(Path_t **list, Vector2 value);
    TINY_BURGER static uint32_t __get_heuristic_value(Vector2 start, Vector2 end);
    TINY_BURGER static void __create_path(const int32_t *const map, Path_t **newPath, Path_t *path, Vector2 value, Vector2 end);
    TINY_BURGER static void __set_path_list(Path_t **list, Vector2 value, uint32_t weight);

    TINY_BURGER static void __destroy(void);
    TINY_BURGER static void __destroy_path(Path_t **ptr);
    TINY_BURGER static uint32_t __get_size_path(Path_t *path);
    TINY_BURGER static void __create_vertor_list(VectorList_t *vectorList, Path_t *path);
    TINY_BURGER static bool __is_fill_open_list(void);

#if defined(__cplusplus)
}
#endif

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER VectorList_t get_path(const int32_t *const map, Vector2 start, Vector2 end)
{
    VectorList_t vectorList = (VectorList_t){0};
    vectorList.vector = NULL;
    vectorList.size = 0;

    Path_t *result = NULL;
    Path_t *path = NULL;

    __init_list();
    __create_path(map, &path, NULL, start, end);
    __push_path_to_list(openList, path);
    while (result == NULL && __is_fill_open_list())
    {
        Path_t *currentPath = __get_path_open_list();
        if (currentPath == NULL)
            break;
        if (currentPath->value.x == end.x && currentPath->value.y == end.y)
            result = currentPath;
        else
            __evaluate_path(map, currentPath, end);
    }

    __create_vertor_list(&vectorList, result);
    __destroy();
    return vectorList;
}

//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER static void __init_list(void)
{
    for (size_t i = 0; i < TINY_BURGER_MAP_HEIGHT; ++i)
    {
        for (size_t j = 0; j < TINY_BURGER_MAP_WIDTH; ++j)
        {
            openList[j + i * TINY_BURGER_MAP_WIDTH] = NULL;
            closeList[j + i * TINY_BURGER_MAP_WIDTH] = NULL;
        }
    }
}

TINY_BURGER static void __evaluate_path(const int32_t *const map, Path_t *path, Vector2 end)
{
    Path_t *up = NULL;
    Path_t *right = NULL;
    Path_t *down = NULL;
    Path_t *left = NULL;

    // up
    if (path->value.y > 0)
    {
        Vector2 value = (Vector2){path->value.x, path->value.y - 1};
        __create_path(map, &up, path, value, end);
    }
    // right
    if (path->value.x < (TINY_BURGER_MAP_WIDTH - 1))
    {
        Vector2 value = (Vector2){path->value.x + 1, path->value.y};
        __create_path(map, &right, path, value, end);
    }
    // down
    if (path->value.y < (TINY_BURGER_MAP_HEIGHT - 1))
    {
        Vector2 value = (Vector2){path->value.x, path->value.y + 1};
        __create_path(map, &down, path, value, end);
    }
    // left
    if (path->value.x > 0)
    {
        Vector2 value = (Vector2){path->value.x - 1, path->value.y};
        __create_path(map, &left, path, value, end);
    }

    __push_path_to_list(openList, up);
    __push_path_to_list(openList, right);
    __push_path_to_list(openList, down);
    __push_path_to_list(openList, left);

    __remove_path_to_list(openList, path);
    __push_path_to_list(closeList, path);
}

TINY_BURGER static void __push_path_to_list(Path_t **list, Path_t *path)
{
    if (path != NULL)
    {
        size_t size = TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT;
        for (size_t i = 0; i < size; ++i)
        {
            if (list[i] == NULL)
            {
                list[i] = path;
                break;
            }
        }
    }
}

TINY_BURGER static bool __exists_value_list(Path_t **list, Vector2 value)
{
    bool result = false;
    size_t size = TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT;

    for (size_t i = 0; i < size; ++i)
    {
        if (list[i] != NULL && list[i]->value.x == value.x && list[i]->value.y == value.y)
        {
            result = true;
            break;
        }
    }

    return result;
}

TINY_BURGER static uint32_t __get_heuristic_value(Vector2 start, Vector2 end)
{
    uint32_t value = fabsf(start.x - end.x) + fabsf(start.y - end.y);
    return value;
}

TINY_BURGER static void __create_path(const int32_t *const map, Path_t **newPath, Path_t *path, Vector2 value, Vector2 end)
{
    uint32_t index = value.x + value.y * TINY_BURGER_MAP_WIDTH;
    int32_t elem = map[index] - 1;
    uint32_t weight = path == NULL ? 0 : path->weight;
    uint32_t heuristic = __get_heuristic_value(value, end);
    uint32_t newWeight = TB_PATH_SIZE + heuristic + weight;

    if (elem >= 0)
    {
        bool existsInCloseList = __exists_value_list(closeList, value);
        bool existsInOpenList = __exists_value_list(openList, value);
        if (!existsInCloseList && !existsInOpenList)
        {
            (*newPath) = (Path_t *)MemAlloc(sizeof(Path_t));
            (*newPath)->weight = newWeight;
            (*newPath)->value = value;
            (*newPath)->prev = path;
        }
        else if (!existsInCloseList && existsInOpenList)
        {
            __set_path_list(openList, value, newWeight);
        }
    }
}

TINY_BURGER static void __remove_path_to_list(Path_t **list, Path_t *path)
{
    size_t size = TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT;
    for (size_t i = 0; i < size; ++i)
    {
        if (list[i] == path)
        {
            list[i] = NULL;
            break;
        }
    }
}

TINY_BURGER static Path_t *__get_path_open_list(void)
{
    Path_t *path = NULL;
    size_t size = TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT;
    uint32_t weight = 10000000;

    for (size_t i = 0; i < size; ++i)
    {
        if (openList[i] != NULL && openList[i]->weight < weight)
        {
            weight = openList[i]->weight;
            path = openList[i];
        }
    }

    return path;
}

TINY_BURGER static void __set_path_list(Path_t **list, Vector2 value, uint32_t weight)
{
    size_t size = TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT;
    for (size_t i = 0; i < size; ++i)
    {
        if (list[i] != NULL && list[i]->value.x == value.x && list[i]->value.y == value.y)
        {
            list[i]->weight = weight;
            break;
        }
    }
}

TINY_BURGER static void __destroy(void)
{
    size_t size = TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT;
    for (size_t i = 0; i < size; ++i)
    {
        __destroy_path(&openList[i]);
        __destroy_path(&closeList[i]);
    }
}

TINY_BURGER static void __destroy_path(Path_t **ptr)
{
    if ((*ptr) != NULL)
    {
        MemFree((*ptr));
        (*ptr) = NULL;
    }
}

TINY_BURGER static uint32_t __get_size_path(Path_t *path)
{
    uint32_t size = 0;
    Path_t *auxPath = path;
    while (auxPath != NULL)
    {
        ++size;
        auxPath = auxPath->prev;
    }

    return size;
}

TINY_BURGER static void __create_vertor_list(VectorList_t *vectorList, Path_t *path)
{
    if (path != NULL)
    {
        uint32_t size = __get_size_path(path);
        if (size > 0)
        {
            vectorList->size = size;
            vectorList->vector = (Vector2 *)MemAlloc(sizeof(Vector2) * size);

            Path_t *auxPath = path;
            uint32_t i = 0;
            while (auxPath != NULL)
            {
                vectorList->vector[i] = auxPath->value;
                ++i;
                auxPath = auxPath->prev;
            }
        }
    }
}

TINY_BURGER static bool __is_fill_open_list(void)
{
    bool isFill = false;
    size_t size = TINY_BURGER_MAP_WIDTH * TINY_BURGER_MAP_HEIGHT;
    for (size_t i = 0; i < size; ++i)
    {
        if (openList[i] != NULL)
        {
            isFill = true;
            break;
        }
    }

    return isFill;
}