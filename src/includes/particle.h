#ifndef __TINY_BURGER_PARTICLE_H__
#define __TINY_BURGER_PARTICLE_H__

#include "config.h"

TINY_BURGER typedef struct
{
    Vector2 position;
    Color color;
    float alpha;
    float size;
    int32_t angle;
    bool active;
} Particle_t;

TINY_BURGER typedef struct
{
    Particle_t **list;
    size_t size;
    bool active;

} ParticleList_t;

#if defined(__cplusplus)
extern "C"
{
#endif

    TINY_BURGER ParticleList_t *create_particle_list(size_t size);
    TINY_BURGER void update_particle_list(ParticleList_t *const particleList);
    TINY_BURGER void draw_particle_list(const ParticleList_t *const particleList);
    TINY_BURGER void active_particle_list(ParticleList_t *const particleList, Vector2 position);
    TINY_BURGER void destroy_particle_list(ParticleList_t **ptr);

#if defined(__cplusplus)
}
#endif

#endif // __TINY_BURGER_PARTICLE_H__