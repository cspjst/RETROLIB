#include "fxp_vec2D_conversions.h"
#include "../fxp_operators.h"
#include "../fxp_trigonometry.h"
#include "../fxp_conversions.h"

cga_point_t fxp_vec2D_to_cga_point(fxp_vector2D_t v) {
    cga_point_t p;
    p.p = v.u;
    return p;
}

fxp_vector2D_t fxp_vec2D_fix_float(float x, float y) {
    fxp_vector2D_t v;
    v.x = fxp_fix_float(x);
    v.y = fxp_fix_float(y);
    return v;
}

fxp_vector2D_t fxp_vec2D_fix_int(int16_t x, int16_t y) {
    fxp_vector2D_t v;
    v.x = fxp_fix_int(x);
    v.y = fxp_fix_int(y);
    return v;
}

// vector transform polar coordinates to the fixed point Cartesian plane
fxp_vector2D_t fxp_vec2D_fix_polar(fxp16_t r, int16_t theta) {
    fxp_vector2D_t u;
    u.x = fxp_mul(r, fxp_cos(theta));
    u.y = fxp_mul(r, fxp_sin(theta));
    return u;
}

fxp_vector2D_t fxp_vec2D_unfix_truncate(fxp_vector2D_t v) {
    fxp_vector2D_t u;
    u.x = fxp_unfix_truncate(v.x);
    u.y = fxp_unfix_truncate(v.y);
    return u;
}

fxp_vector2D_t fxp_vec2D_unfix_ceiling(fxp_vector2D_t v) {
    fxp_vector2D_t u;
    u.x = fxp_unfix_ceiling(v.x);
    u.y = fxp_unfix_ceiling(v.y);
    return u;
}

fxp_vector2D_t fxp_vec2D_unfix_floor(fxp_vector2D_t v) {
    fxp_vector2D_t u;
    u.x = fxp_unfix_floor(v.x);
    u.y = fxp_unfix_floor(v.y);
    return u;
}

fxp_vector2D_t fxp_vec2D_unfix_round(fxp_vector2D_t v) {
    fxp_vector2D_t u;
    u.x = fxp_unfix_round(v.x);
    u.y = fxp_unfix_round(v.y);
    return u;
}
