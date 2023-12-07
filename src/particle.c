#include "includes/particle.h"

//----------------------------------------------------------------------------------
// Global.
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Static Definition.
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Public Functions Implementation.
//----------------------------------------------------------------------------------
TINY_BURGER ParticleList_t *create_particle_list(size_t size)
{
    ParticleList_t *particleList = (ParticleList_t *)MemAlloc(sizeof(ParticleList_t));
    if (particleList == NULL)
    {
        TraceLog(LOG_DEBUG, "Error to create ParticleList_t pointer.");
        return NULL;
    }

    particleList->list = (Particle_t **)MemAlloc(sizeof(Particle_t *) * size);
    if (particleList->list == NULL)
    {
        MemFree(particleList);
        particleList = NULL;

        TraceLog(LOG_DEBUG, "Error to create Particle_t pointer.");
        return NULL;
    }

    for (size_t i = 0; i < size; ++i)
    {
        particleList->list[i] = (Particle_t *)MemAlloc(sizeof(Particle_t));
        particleList->list[i]->position = (Vector2){0};
        particleList->list[i]->color = GetColor(TINY_BURGER_COLOR_2);
        particleList->list[i]->alpha = 1.0;
        particleList->list[i]->size = GetRandomValue(2, 4);
        particleList->list[i]->angle = GetRandomValue(0, 360) * DEG2RAD;
        particleList->list[i]->active = false;
    }
    particleList->size = size;
    particleList->active = false;

    TraceLog(LOG_DEBUG, "ParticleList_t pointer list created successfully.");
    return particleList;
}

TINY_BURGER void update_particle_list(ParticleList_t *const particleList)
{
    if (particleList->active)
    {
        bool active = false;
        for (size_t i = 0; i < particleList->size; ++i)
        {
            if (particleList->list[i]->active)
            {
                active = true;
                int32_t angle = particleList->list[i]->angle;
                particleList->list[i]->position.y += sinf(angle) * GetRandomValue(5, 9) / 10;
                particleList->list[i]->position.x += cosf(angle) * GetRandomValue(5, 9) / 10;
                particleList->list[i]->alpha -= 0.04f;
                particleList->list[i]->active = !(particleList->list[i]->alpha < 0);
            }
        }
        particleList->active = active;
    }
}

TINY_BURGER void draw_particle_list(const ParticleList_t *const particleList)
{
    if (particleList->active)
    {
        for (size_t i = 0; i < particleList->size; ++i)
        {
            if (particleList->list[i]->active)
            {
                DrawRectangle(
                    particleList->list[i]->position.x,
                    particleList->list[i]->position.y,
                    particleList->list[i]->size,
                    particleList->list[i]->size,
                    Fade(particleList->list[i]->color, particleList->list[i]->alpha));
            }
        }
    }
}

TINY_BURGER void active_particle_list(ParticleList_t *const particleList, Vector2 position)
{
    particleList->active = true;
    for (size_t i = 0; i < particleList->size; i++)
    {
        particleList->list[i]->position = position;
        particleList->list[i]->alpha = 1.0;
        particleList->list[i]->active = true;
    }
}

TINY_BURGER void destroy_particle_list(ParticleList_t **ptr)
{
    if ((*ptr) != NULL)
    {
        for (size_t i = 0; i < (*ptr)->size; ++i)
        {
            MemFree((*ptr)->list[i]);
            (*ptr)->list[i] = NULL;
            TraceLog(LOG_DEBUG, "Particle_t pointer destroyed successfully.");
        }

        MemFree((*ptr)->list);
        (*ptr)->list = NULL;

        MemFree((*ptr));
        (*ptr) = NULL;
        TraceLog(LOG_DEBUG, "ParticleList_t pointer destroyed successfully.");
    }
}
//----------------------------------------------------------------------------------
// Static Functions Implementation.
//----------------------------------------------------------------------------------