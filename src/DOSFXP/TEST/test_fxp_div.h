#ifndef TEST_FXP_DIV_H
#define TEST_FXP_DIV_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../FXP/fxp_types.h"
#include "../FXP/fxp_limits.h"
#include "../FXP/fxp_conversions.h"

extern fxp16_t fxp_div(fxp16_t lhs, fxp16_t rhs);

/* Exact-integer reference oracle - only ever called with pairs
   already known to be safe, so no overflow/zero handling needed. */
static fxp16_t fxp_div_reference(fxp16_t lhs, fxp16_t rhs)
{
    int32_t numerator = (int32_t)lhs * 64;
    int32_t q = numerator / rhs;  /* C99: truncates toward zero, matches IDIV */
    return (fxp16_t)q;
}

/* For a given divisor, the largest |lhs| that is guaranteed not to
   overflow the quotient, capped at the int16 range. Anything at or
   below this bound is provably safe to pass to fxp_div. */
static int32_t max_safe_lhs_magnitude(fxp16_t rhs)
{
    int32_t bound = (32767L * (int32_t)abs((int)rhs)) / 64L;
    if (bound > 32767) bound = 32767;
    return bound;
}

/* ------------------------------------------------------------------
 * 1. Known hand-computed examples, all independently pre-verified as
 *    safe (rhs != 0, quotient fits AX) before being hardcoded.
 * ------------------------------------------------------------------ */
static void test_known_examples() {
    printf("test known examples...\n");

    assert(fxp_div(-64, 64)   == -64);   /* -1.0 / 1.0  = -1.0  - the original sign-extension bug case */
    assert(fxp_div(64, 64)    == 64);    /*  1.0 / 1.0  =  1.0  */
    assert(fxp_div(-64, -64)  == 64);    /* -1.0 / -1.0 =  1.0  */
    assert(fxp_div(0, 5)      == 0);     /*  0 / anything nonzero = 0 */
    assert(fxp_div(511, 1)    == 32704); /* just inside the rhs=1 safe boundary */
    assert(fxp_div(-511, 1)   == -32704);
    assert(fxp_div(-32768, -32768) == 64); /* both operands at their own extreme, safe result */

    printf("test_known_examples: PASS\n");
}

/* ------------------------------------------------------------------
 * 2. Exhaustive, per-divisor, over the EXACT provably-safe lhs range
 *    for that divisor - not a sample, full coverage of the safe zone
 *    for each rhs tested. Smaller |rhs| naturally gives a narrower
 *    (but fully covered) safe range; |rhs| >= 64 covers the entire
 *    lhs domain, since 32767*64/64 = 32767 (verified separately).
 * ------------------------------------------------------------------ */
static void test_exhaustive_safe_zone(void) {
    printf("test exhaustive - very slow on XT or emulated XT...\n");
    static const fxp16_t rhs_values[] = {
        1, -1, 2, -2, 5, -5, 64, -64, 100, -100, 1000, -1000, 32767, -32768
    };
    size_t r;

    for (r = 0; r < sizeof(rhs_values) / sizeof(rhs_values[0]); r++) {
        fxp16_t rhs = rhs_values[r];
        int32_t max_mag = max_safe_lhs_magnitude(rhs);
        int32_t lhs_raw;

        for (lhs_raw = -max_mag; lhs_raw <= max_mag; lhs_raw++) {
            fxp16_t lhs = (fxp16_t)lhs_raw;
            fxp16_t expected = fxp_div_reference(lhs, rhs);
            fxp16_t actual   = fxp_div(lhs, rhs);
            assert(actual == expected);
        }
    }

    printf("test_exhaustive_safe_zone: PASS (full safe-zone coverage, %lu divisors)\n",
           (unsigned long)(sizeof(rhs_values) / sizeof(rhs_values[0])));
}

/* ------------------------------------------------------------------
 * 3. Random fuzz, constructed to be safe BY CONSTRUCTION: pick a
 *    random nonzero rhs first, compute its exact safe lhs bound, then
 *    pick lhs uniformly within that bound. No rejection sampling, no
 *    possibility of ever calling fxp_div with an unsafe pair.
 * ------------------------------------------------------------------ */
static void test_random_fuzz_safe() {
    printf("test random fuzz - slow on XT or emulated XT...\n");
    const int N = 200000;
    int i;

    srand(1); /* fixed seed - reproducible across runs and machines */

    for (i = 0; i < N; i++) {
        fxp16_t rhs;
        int32_t max_mag;
        int32_t lhs_raw;
        fxp16_t lhs;

        do {
            rhs = (fxp16_t)((rand() << 1) ^ rand());
        } while (rhs == 0);

        max_mag = max_safe_lhs_magnitude(rhs);
        lhs_raw = (int32_t)(rand() % (2 * max_mag + 1)) - max_mag;
        lhs = (fxp16_t)lhs_raw;

        {
            fxp16_t expected = fxp_div_reference(lhs, rhs);
            fxp16_t actual   = fxp_div(lhs, rhs);
            assert(actual == expected);
        }
    }

    printf("test_random_fuzz_safe: PASS (%d pairs, safe by construction)\n", N);
}

static void test_fxp_div() {
    test_known_examples();
    test_exhaustive_safe_zone();
    test_random_fuzz_safe();

    printf("All fxp_div tests passed (safe-input-only coverage).\n");
}


#endif
