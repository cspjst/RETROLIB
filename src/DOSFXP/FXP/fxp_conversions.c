/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 */
#include "fxp_conversions.h"
#include "fxp_parts.h"
#include "fxp_types.h"
#include "fxp_constants.h"
#include "fxp_limits.h"
#include <stdio.h>



fxp16_t fxp_fix_float(float f) {
    float scaled = f * (float)FXP_ONE;
    return (fxp16_t)(scaled >= 0.0f ? scaled + 0.5f : scaled - 0.5f);
}

/**
 * Saturation arithmetic avoids the dangers of modular wrap around by clamping storage size overflows to maximum or minimum
 * representation of the storage size i.e. a local +infinity and -infinity aka FXP_MAX and FXP_MIN.
 */
fxp16_t fxp_saturate(int32_t n) {
    if (n > FXP_INFINITY)  return (fxp16_t)FXP_MAX;
    if (n < FXP_NINFINITY) return (fxp16_t)FXP_MIN;
    return (fxp16_t)n;
}

/**
 * Truncate toward zero: bias negative values by FXP_FRACTIONAL_MASK before the arithmetic shift
 * Use standard signed-truncating-division-by-power-of-2 trick.
 * NB 1. Needs a conditional (sign test), hence a plain function with an __asm block rather than #pragma aux.
 * NB 2. Uses Watcom calling convention returning in AX
 */
int16_t fxp_unfix_truncate(fxp16_t x) {
    __asm {
        .8086
        or      ax, ax
        jns     L1                      ; signed?
        add     ax, FXP_PART_FRAC_MASK
L1:     sar     ax, 1
        sar     ax, 1
        sar     ax, 1
        sar     ax, 1
        sar     ax, 1
        sar     ax, 1
    }
}

/**
 * Ceiling toward +infinity: floor, then +1 if any fractional bit was set, regardless of sign.
 */
int16_t fxp_unfix_ceiling(fxp16_t x) {
    __asm {
        .8086
        mov     cx, x                   ; copy ax for test
        sar     ax, 1                   ; 8086 restricted to single shifts
        sar     ax, 1
        sar     ax, 1
        sar     ax, 1
        sar     ax, 1
        sar     ax, 1
        test    cx, FXP_PART_FRAC_MASK  ; any fractional bits?
        jz      END
        inc     ax                      ; +1 if any fractional bit was set
END:
    }
}
