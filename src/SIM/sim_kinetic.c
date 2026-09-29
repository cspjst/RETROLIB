#include "sim_kinetic.h"

sim_kinetic_t sim_kinetic_make(fxp_vector2D_t p, fxp_vector2D_t v, fxp_vector2D_t a) {
    sim_kinetic_t k;
    k.position = p;
    k.velocity = v;
    k.acceleration = a;
    return k;
}
