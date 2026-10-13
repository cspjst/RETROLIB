#include "sim_constraints.h"

#include "../FXP/fxp_conversions.h"

#include <stdio.h>

sim_constraints_t sim_constraints_make(fxp_rectangle_t r, fxp_interval_t v, fxp_interval_t a) {
    sim_constraints_t c;
    c.position_bounds = r;
    c.velocity_bounds = v;
    c.acceleration_bounds = a;
    return c;
}

char* sim_constraints_str(sim_constraints_t* c) {
    static char buf[96];
    sprintf(buf, "{(%.3f,%.3f,%.3f,%.3f) (%.3f,%.3f) (%.3f,%.3f)}",
            (double)fxp_unfix_float(c->position_bounds.x),
            (double)fxp_unfix_float(c->position_bounds.y),
            (double)fxp_unfix_float(c->position_bounds.w),
            (double)fxp_unfix_float(c->position_bounds.h),
            (double)fxp_unfix_float(c->velocity_bounds.i),
            (double)fxp_unfix_float(c->velocity_bounds.j),
            (double)fxp_unfix_float(c->acceleration_bounds.parts[0]),
            (double)fxp_unfix_float(c->acceleration_bounds.parts[1]));
    return buf;
}
