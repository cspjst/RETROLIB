/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 * @brief macro definitions for accessing the whole and fractional parts of the 10:6 fixed point type
 */
#ifndef FXP_PARTS_H
#define FXP_PARTS_H

#include "fxp_types.h"

#define fxp_part_whole(v) ((fxp16_t)((v) & FXP_PART_WHOLE_MASK) >> 6)

#define fxp_part_frac(v) ((fxp16_t)((v) & FXP_PART_FRAC_MASK))

#endif
