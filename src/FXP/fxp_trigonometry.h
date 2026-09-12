/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 * @brief Fast Fixed-Point Trig by Binary Angular Measurement (BAM) 8bit 64 byte Table-Lookup
 * @url https://en.wikipedia.org/wiki/Binary_angular_measurement.
 *
 * @note 1. The trigonometry functions deliberately take an integer angle in degrees.
 * The caller explicitly picks fxp_unfix_floor/_round/_trunc beforehand.
 *
 * @note 2. A BAM8 256 steps/revolution circle quadrant table of sine values is 64 entries which is the same
 * fractional resolution as 6bits of the fxp16_t 10:6 fixed point number
 */
#ifndef FXP_TRIGONOMETRY_H
#define FXP_TRIGONOMETRY_H

#include "fxp_types.h"

#define FXP_BAM8_ANGLE_BITS         8
#define FXP_BAM8_CIRCLE             256   // steps/revolution
#define FXP_BAM8_QUADRANT           64    // 0..90 degrees
#define FXP_BAM8_QUADRANT_BITS      6
#define FXP_BAM8_QUADRANT_MASK      0x3F


fxp16_t fxp_sin(int16_t degrees);

fxp16_t fxp_cos(int16_t degrees);

#endif
