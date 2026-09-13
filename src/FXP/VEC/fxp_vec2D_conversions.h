#ifndef FXP_VEC2D_CONVERSIONS_H
#define FXP_VEC2D_CONVERSIONS_H

#include <stdint.h>
#include "../fxp_types.h"
#include "../VEC/fxp_vec2d_types.h"
#include "../../CGA/cga_types.h"

cga_point_t fxp_vec2D_to_cga_point(fxp_vector2D_t v);

fxp_vector2D_t fxp_vec2D_fix_float(float x, float y);

fxp_vector2D_t fxp_vec2D_fix_int(int16_t x, int16_t y);

// vector transform polar coordinates to the fixed point Cartesian plane
fxp_vector2D_t fxp_vec2D_fix_polar(fxp16_t r, int16_t theta);

fxp_vector2D_t fxp_vec2D_unfix_truncate(fxp_vector2D_t v);

fxp_vector2D_t fxp_vec2D_unfix_ceiling(fxp_vector2D_t v);

fxp_vector2D_t fxp_vec2D_unfix_floor(fxp_vector2D_t v);

fxp_vector2D_t fxp_vec2D_unfix_round(fxp_vector2D_t v);

#endif
