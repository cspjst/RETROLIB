/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * Generic motion with vectors type
 */
#ifndef SIM_KINETIC_H
#define SIM_KINETIC_H

#include "../FXP/fxp_types.h"
#include "../FXP/fxp_rectangle.h"
#include "../FXP/VEC/fxp_vec2D_types.h"

typedef struct {
    fxp_vector2D_t position;
    fxp_vector2D_t velocity;
    fxp_vector2D_t acceleration;
} sim_kinetic_t;

typedef struct {
    fxp_rectangle_t position_bounds;
    fxp16_interval_t velocity_bounds;
    fxp16_interval_t acceleration_bounds;
} sim_kinetic_constraints_t;

typedef int sim_kinetic_constrainer(sim_kinetic_t* k, const sim_kinetic_constraints_t* c);

int sim_kinetic_update(sim_kinetic_t* k, const sim_kinetic_constraints_t* c, sim_kinetic_constrainer* f);

#endif
