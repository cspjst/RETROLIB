#ifndef SIM_PARTICLE_H
#define SIM_PARTICLE_H

#include "sim_kinetic.h"

typedef struct {
    sim_kinetic_t kinetic;
    fxp16_t lifespan;
    fxp16_t mass;
    fxp16_t radius;
} sim_particle_t;

#endif
