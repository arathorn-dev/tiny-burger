#ifndef __TINY_BURGER_PATHFINDING_H__
#define __TINY_BURGER_PATHFINDING_H__

#include "config.h"

TINY_BURGER typedef struct
{
    Vector2 value;
    uint32_t weight;
    void *prev;
} Path_t;

TINY_BURGER typedef struct
{
    Vector2 *vector;
    uint32_t size;
} VectorList_t;

#if defined(__cplusplus)
extern "C"
{
#endif
    TINY_BURGER void get_path(Vector2 *vectorList, const int32_t *const map, Vector2 start, Vector2 end);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_PATHFINDING_H__
