#ifndef FXP_DIMENSION_H
#define FXP_DIMENSION_H

#include "fxp_types.h"

typedef union {
    int32_t u;
    struct {
        fxp16_t w;
        fxp16_t h;
    };
} fxp16_dimension_t;

#endif
