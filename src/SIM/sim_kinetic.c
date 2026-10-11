#include "sim_kinetic.h"

#include "../FXP/fxp_conversions.h"

#include <stdio.h>

sim_kinetic_t sim_kinetic_make(fxp_vector2D_t p, fxp_vector2D_t v, fxp_vector2D_t a) {
    sim_kinetic_t k;
    k.position = p;
    k.velocity = v;
    k.acceleration = a;
    return k;
}

char* sim_kinetic_str(sim_kinetic_t* k) {
    static char buf[96];
    sprintf(buf, "{(%.3f,%.3f) (%.3f,%.3f) (%.3f,%.3f)}",
            (double)fxp_unfix_float(k->position.x),
            (double)fxp_unfix_float(k->position.y),
            (double)fxp_unfix_float(k->velocity.x),
            (double)fxp_unfix_float(k->velocity.y),
            (double)fxp_unfix_float(k->acceleration.x),
            (double)fxp_unfix_float(k->acceleration.y));
    return buf;
}
