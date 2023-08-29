// #include "includes/app.h"

// int main(void)
// {
//     App_t *app = create_app();
//     if (app == NULL)
//     {
//         return 1;
//     }
//     run_app(app);
//     destroy_app(&app);
//     return 0;
// }

#include <stdio.h>
#include "includes/config.h"
#include "includes/pathfinding.h"

Vector2 start = {2, 3};
Vector2 end = {3, 1};

uint32_t map[36] = {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 0,
    0, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0};

void print_map(void);

int *list[2] = {NULL, NULL};

int main(void)
{
    VectorList_t path = get_path(map, start, end);
    if (path.vector == NULL)
    {
        TraceLog(LOG_INFO, "ES NULL :(");
    }
    else
    {
        for (int32_t i = (path.size - 1); i >= 0; --i)
        {
            Vector2 value = path.vector[i];
            TraceLog(LOG_INFO, TextFormat("[x: %f, y: %f]", value.x, value.y));
        }
    }

    return 0;
}

void print_map(void)
{
    for (size_t i = 0; i < 6; ++i)
    {
        for (size_t j = 0; j < 6; ++j)
        {
            printf("%d ", map[j + i * 6]);
        }
        printf("\n");
    }
}