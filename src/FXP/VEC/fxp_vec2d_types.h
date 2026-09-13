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

#endif
