#ifndef FXP_VEC2D_TYPES_H
#define FXP_VEC2D_TYPES_H

#include <stdint.h>
#include "../fxp_types.h"

typedef union {
    uint32_t u;
    struct {
        fxp16_t i;      // horizonatal x
        fxp16_t j;      // vertical y
    };
} fxp_vector2D_t;

#endif
