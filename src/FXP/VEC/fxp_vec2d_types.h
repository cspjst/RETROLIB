#ifndef FXP_VEC2D_TYPES_H
#define FXP_VEC2D_TYPES_H

#include <stdint.h>
#include "../fxp_types.h"

/**
 * A fixed point 10:6 Cartesian plane rectangular coordinate vector
 */
typedef union {
    uint32_t u;
    struct {
        fxp16_t x;
        fxp16_t y;
    };
} fxp_vector2D_t;

static const fxp_vector2D_t fxp_vec2D_null = {0};

static const fxp_vector2D_t fxp_vec2D_unit = {0x40};

const char* fxp_vec2D_str(fxp_vector2D_t v);

#endif
