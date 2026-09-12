/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 * @brief
 */
#ifndef FXP_OPERATORS_H
#define FXP_OPERATORS_H

#include "fxp_types.h"

fxp16_t fxp_add(fxp16_t lhs, fxp16_t rhs);

fxp16_t fxp_sub(fxp16_t lhs, fxp16_t rhs);

fxp16_t fxp_mul(fxp16_t lhs, fxp16_t rhs);

fxp16_t fxp_div(fxp16_t lhs, fxp16_t rhs);

inline fxp16_t fxp_pow2(fxp16_t x) { return fxp_mul(x, x); }

fxp16_t fxp_sqrt(fxp16_t x);

fxp16_t fxp_mod(fxp16_t x, fxp16_t y);

#endif
