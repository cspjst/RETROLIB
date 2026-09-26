/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 * fxp_exp() function lookup e (Euler's Number = 2.71828) raised to the power x where x is any fxp16_t value
 *
 * @note Domain: [-4.84375, +6.234375] raw fxp16_t units 710 entries, 1420 bytes
 * Both bounds limited by fxp16_t own 1/64 resolution:
 * + Below raw -310, exp(x) < 0.5/64 and rounds to 0 regardless of table precision
 * + Above raw +399, exp(x) exceeds FXP_MAX (511.984375) - overflow
 *
 * @warning Input outside [-4.84375, +6.234375] domain is a genuine caller error and triggers a deliberate INT 0 panic!
 */
#ifndef FXP_EXP_LOOKUP_H
#define FXP_EXP_LOOKUP_H

#include "fxp_types.h"

#define FXP_EXP_NEG_BOUND   -310
#define FXP_EXP_POS_BOUND   399
#define FXP_EXP_ENTRIES     710

fxp16_t fxp_exp(fxp16_t x);

#endif
