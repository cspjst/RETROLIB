#ifndef FXP_POINT_H
#define FXP_POINT_H

#include "fxp_types.h"

typedef union {
    int32_t u;
    struct {
        fxp16_t x;
        fxp16_t y;
    };
} fxp16_point_t;

fxp16_t* fxp_point_translate(fxp16_t dx, fxp16_t dy);

#endif
