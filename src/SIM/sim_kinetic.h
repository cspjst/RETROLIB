/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * Generic motion with vectors type
 */
#ifndef SIM_KINETIC_H
#define SIM_KINETIC_H

#include "../FXP/VEC/fxp_vec2_types.h"

typedef struct {
    fxp_vector2D_t position;
    fxp_vector2D_t velocity;
    fxp_vector2D_t acceleration;    
} sim_kinetic_t;

int sim_kinetic_update(sim_kinetic_t* k);

#endif 
