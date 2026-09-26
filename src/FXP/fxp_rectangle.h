#ifndef FXP_RECTANGLE_H
#define FXP_RECTANGLE_H

#include "fxp_types.h"

typedef union {
    int64_t u;
    struct {
        fxp16_t x;
        fxp16_t y;
        fxp16_t w;
        fxp16_t h;
    };
} fxp_rectangle_t;

#endif
