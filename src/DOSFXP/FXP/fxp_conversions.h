/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 * @brief  Numeric conversions for fxp16_t
 *
 * Conversion functions to and from the the C <float> number line to the <fxp16_t> fixed point -512..+511.984357 number line
 */
#ifndef FXP_CONVERSIONS_H
#define FXP_CONVERSIONS_H

#include "fxp_types.h"
#include "fxp_constants.h"

// entry to the 10:6 fixed point number line int->fxp16_t
inline fxp16_t fxp_fix_int(int i) { return i << 6; }

// entry to the 10:6 fixed point number line float->fxp16_t
fxp16_t fxp_fix_float(float f);

// entry to the 10:6 fixed point number line, clamped narrowing of an already-rescaled 32-bit intermediate
// (e.g. a multiply result) -> fxp16_t
fxp16_t fxp_saturate(int32_t n);

// exit from the 10:6 fixed point number line fxp16_t->float
inline float fxp_unfix_float(fxp16_t x)  {
    return (float)x / (float)FXP_ONE;
}

// exit from the 10:6 fixed point number line fxp16_t->int16_t, toward zero
int16_t fxp_unfix_truncate(fxp16_t x);

// exit from the 10:6 fixed point number line fxp16_t->int16_t, toward +infinity
int16_t fxp_unfix_ceiling(fxp16_t x);

// exit from the 10:6 fixed point number line fxp16_t->int16_t, toward -infinity
int16_t fxp_unfix_floor(fxp16_t x);
// The #pragma aux directive in Watcom C/C++ is used to define inline assembly code
// Here the performance advantage is gained by sacriicing 12 bytes for no call/return overhead
// Six signed arithmetic shift right due to limitations of 8086/8 instruction set
#pragma aux fxp_unfix_floor =   \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    parm [ax]                   \
    value [ax]                  \
    modify [ax];

// exit from the 10:6 fixed point number line fxp16_t->int16_t, nearest, ties toward +infinity
extern int16_t fxp_unfix_round(fxp16_t v);
// The #pragma aux directive in Watcom C/C++ is used to define inline assembly code
// Here the performance advantage is gained by sacriicing 15 bytes for no call/return overhead
#pragma aux fxp_unfix_round =   \
    "add ax, 32"                \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    "sar ax, 1"                 \
    parm [ax]                   \
    value [ax]                  \
    modify [ax];

#endif
