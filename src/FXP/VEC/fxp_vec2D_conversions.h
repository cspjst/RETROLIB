#ifndef FXP_VEC2D_CONVERSIONS_H
#define FXP_VEC2D_CONVERSIONS_H

#include <stdint.h>
#include "../fxp_types.h"
#include "../fxp_operators.h"
#include "../fxp_trigonometry.h"
#include "../VEC/fxp_vec2d_types.h"

// vector transform polar coordinates to the fixed point Cartesian plane
inline void fxp_vec2D_fix_polar(fxp_vector2D_t* v, fxp16_t r, int16_t theta) {
    v->x = fxp_mul(r, fxp_cos(theta));
    v->y = fxp_mul(r, fxp_sin(theta));
}

#endif
