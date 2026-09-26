/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 * fxp_sin() function lookup sine value of an *integer* angle
 *
 * @url https://en.wikipedia.org/wiki/Binary_angular_measurement
 * Binary Angular Measurement (BAM) 14 bit quarter-sine lookup table data
 * Raw 361 entry fxp16_t sine table for 0-360 degrees (interpolated from the 256 entry BAM8 table)
 * Values are 6-bit fractional 10:6 fixed point (1.0 = 64, -1.0 = -64)
 */
#ifndef FXP_SIN_LOOKUP_H
#define FXP_SIN_LOOKUP_H

#include "fxp_types.h"

#define FXP_SIN_ENTRIES     361

fxp16_t fxp_sin(int16_t degrees);

#endif
