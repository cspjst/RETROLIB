/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 * @brief 10:6 signed fixed point, one type to rule them all (CGA, HGA, VGA)
 *
 * DOSFXP defines a single 16-bit signed fixed-point type, fxp16_t,
 * using a 10:6 split (10 integer bits, 6 fractional bits, two's complement), stored as int16_t.
 * The format yields a representable range of -512.0 to +511.984375, with a fractional resolution of 1/64 (0.015625).
 * This range was derived empirically from candidate DOS display resolutions CGA, HGA and VGA.
 * In this format pi approximates to 201/64 = 3.140625 with a relative error approximately = 0.031%.
 */
#ifndef FXP_TYPES_H
#define FXP_TYPES_H

#define FXP_INTEGRAL_BITS    10
#define FXP_FRACTIONAL_BITS  6

#define FXP_PART_WHOLE_MASK  0xFFC0    // 1111111111.000000
#define FXP_PART_FRAC_MASK   0x3F      // 0000000000.111111

#include <stdint.h>

typedef int16_t fxp16_t;

#endif
