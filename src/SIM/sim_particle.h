#ifndef SIM_PARTICLE_H
#define SIM_PARTICLE_H

#include "../FXP/VEC/fxp_vec2_types.h"

typedef struct {
    fxp_vector2D_t position;
    fxp_vector2D_t velocity;
    fxp_vector2D_t acceleration;    
} sim_particle_t;

#endif
