#include "fxp_vec2D_types.h"

#include <stdio.h>

#include "../fxp_conversions.h"

const char* fxp_vec2D_str(fxp_vector2D_t v) {
    static char buf[21];   // "(-512.000, -512.000)" + NUL
    sprintf(buf, "(%.3f, %.3f)", (double)fxp_unfix_float(v.x), (double)fxp_unfix_float(v.y));
    return buf;
}
