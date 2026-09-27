#ifndef FXP_VEC2D_OPERATORS_H
#define FXP_VEC2D_OPERATORS_H

#include "fxp_vec2D_types.h"

fxp_vector2D_t fxp_vec2D_add(fxp_vector2D_t a, fxp_vector2D_t b);

/**
 * Uniform Cartesian x,y scale
 */
fxp_vector2D_t fxp_vec2D_scale(fxp_vector2D_t v, fxp16_t scalar);

/**
 * A Hadamard product scale - x and y move by separate factors
 * @url https://en.wikipedia.org/wiki/Hadamard_product_(matrices)
 */
fxp_vector2D_t fxp_vec2D_scale_xy(fxp_vector2D_t v, fxp16_t sx, fxp16_t sy);

#endif
