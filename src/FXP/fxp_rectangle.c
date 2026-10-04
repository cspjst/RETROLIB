#include "fxp_rectangle.h"
#include "fxp_types.h"
#include "fxp_conversions.h"

#include <stdio.h>

fxp_rectangle_t fxp_rectangle_make(fxp16_t x, fxp16_t y, fxp16_t w, fxp16_t h) {
    fxp_rectangle_t r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

char* fxp_rectangle_str(fxp_rectangle_t r) {
    static char buf[48];   // "{x:-512.000 y:-512.000 w:-512.000 h:-512.000}" + NUL = 46
    sprintf(buf, "{x:%.3f y:%.3f w:%.3f h:%.3f}",
            (double)fxp_unfix_float(r.x),
            (double)fxp_unfix_float(r.y),
            (double)fxp_unfix_float(r.w),
            (double)fxp_unfix_float(r.h));
    return buf;
}
