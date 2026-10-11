#ifndef SIM_CONSTRAINTS_H
#define SIM_CONSTRAINTS_H

#include "../FXP/fxp_types.h"
#include "../FXP/fxp_rectangle.h"

typedef struct {
    fxp_rectangle_t position_bounds;
    fxp_interval_t velocity_bounds;
    fxp_interval_t acceleration_bounds;
} sim_constraints_t;

sim_constraints_t sim_constraints_make(fxp_rectangle_t r, fxp_interval_t v, fxp_interval_t a);

sim_constraints_t* sim_constraints_new(fxp_rectangle_t r, fxp_interval_t v, fxp_interval_t a);

char* sim_constraints_str(sim_constraints_t* c);

#endif
