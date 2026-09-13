#include "fxp_vec2D_operators.h"
#include "../fxp_operators.h"

fxp_vector2D_t fxp_vec2D_add(fxp_vector2D_t a, fxp_vector2D_t b) {
    fxp_vector2D_t r;
    r.x = fxp_add(a.x, b.x);
    r.y = fxp_add(a.y, b.y);
    return r;
}

fxp_vector2D_t fxp_vec2D_scale(fxp_vector2D_t v, fxp16_t scalar) {
    fxp_vector2D_t r;
    r.x = fxp_mul(v.x, scalar);
    r.y = fxp_mul(v.y, scalar);
    return r;
}
