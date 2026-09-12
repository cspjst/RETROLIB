#ifndef TEST_FXP_MUL_H
#define TEST_FXP_MUL_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../FXP/fxp_constants.h"
#include "../FXP/fxp_limits.h"
#include "../FXP/fxp_conversions.h"
#include "../FXP/fxp_operators.h"

/* Exact-integer reference oracle - mirrors the asm's DX:AX logic
   bit-for-bit using native 32-bit arithmetic instead of hand-rolled
   shift emulation, as an independent cross-check. */
static fxp16_t fxp_mul_reference(fxp16_t lhs, fxp16_t rhs)
{
    int32_t product = (int32_t)lhs * (int32_t)rhs;
    int16_t hi = (int16_t)(product >> 16);  /* matches DX exactly */

    if (hi > 31)  return (fxp16_t)FXP_MAX;
    if (hi < -32) return (fxp16_t)FXP_MIN;

    return (fxp16_t)(product >> 6);            /* arithmetic shift, floors */
}

/* ------------------------------------------------------------------
 * 1. Known hand-computed examples: identity, negation, fractional,
 *    zero - the sanity checks a human would do first.
 * ------------------------------------------------------------------ */
static void test_known_examples(void)
{
    /* raw values: FXP_ONE = 64 = 1.0 */
    assert(fxp_mul(64, 64)   == 64);     /* 1.0 * 1.0  = 1.0  */
    assert(fxp_mul(64, -64)  == -64);    /* 1.0 * -1.0 = -1.0 */
    assert(fxp_mul(-64, -64) == 64);     /* -1.0 * -1.0 = 1.0 */
    assert(fxp_mul(0, 12345) == 0);      /* 0 * anything = 0  */
    assert(fxp_mul(128, 32)  == 64);     /* 2.0 * 0.5 = 1.0   */
    assert(fxp_mul(64, 32767) == 32767); /* 1.0 * FXP_MAX = FXP_MAX (identity) */

    printf("test_known_examples: PASS\n");
}

/* ------------------------------------------------------------------
 * 2. Exact boundary pairs, hand-derived and verified against the
 *    algebraic proof that DX>31 / DX<-32 is an EXACT threshold (no
 *    partial-overflow case within a given DX value - proven
 *    separately, not just tested empirically here).
 * ------------------------------------------------------------------ */
static void test_overflow_boundary_exact(void)
{
    /* lhs=128 (2.0), rhs=16383 (255.984375): product hi=31, just safe */
    assert(fxp_mul(128, 16383) == 32766);

    /* lhs=128 (2.0), rhs=16384 (256.0): product hi=32, just clamps */
    assert(fxp_mul(128, 16384) == FXP_MAX);

    /* lhs=128, rhs=-16384: product hi=-32 exactly, still "safe" by
       the routine's own threshold - falls through to the plain shift
       and produces raw FXP_MIN (-32768) without ever hitting the
       clamp path. This is the expected, settled behaviour: FXP_MIN
       is a legitimately reachable non-overflow result. */
    assert(fxp_mul(128, -16384) == FXP_MIN);

    /* lhs=128, rhs=-16385: product hi=-33, just clamps */
    assert(fxp_mul(128, -16385) == FXP_MIN);

    printf("test_overflow_boundary_exact: PASS\n");
}

/* ------------------------------------------------------------------
 * 3. Clearly-in-overflow cases, well away from the boundary, to
 *    catch gross sign or comparison-direction errors independently
 *    of the exact-threshold tests above.
 * ------------------------------------------------------------------ */
static void test_overflow_gross(void)
{
    assert(fxp_mul(16384, 16384) == FXP_MAX);      /* 256.0 * 256.0, wildly over range */
    assert(fxp_mul(-16384, 16384) == FXP_MIN); /* -256.0 * 256.0, wildly under range */

    printf("test_overflow_gross: PASS\n");
}

/* ------------------------------------------------------------------
 * 4. Exhaustive over lhs (all 65536 values) against a fixed set of
 *    representative rhs constants - full single-operand coverage,
 *    not a sample.
 * ------------------------------------------------------------------ */
static void test_exhaustive_by_lhs() {
    printf("test exhaustive - very slow on XT or emulated XT...\n");
    static const fxp16_t rhs_values[] = {
        0, 64, -64, 128, -128, 1, -1, 32767, -32768, 32, -32
    };
    size_t r;
    int32_t lhs_raw;

    for (r = 0; r < sizeof(rhs_values) / sizeof(rhs_values[0]); r++) {
        fxp16_t rhs = rhs_values[r];

        for (lhs_raw = INT16_MIN; lhs_raw <= INT16_MAX; lhs_raw++) {
            fxp16_t lhs = (fxp16_t)lhs_raw;
            fxp16_t expected = fxp_mul_reference(lhs, rhs);
            fxp16_t actual   = fxp_mul(lhs, rhs);
            assert(actual == expected);
        }
    }

    printf("test_exhaustive_by_lhs: PASS (65536 x %lu rhs values)\n",
           (unsigned long)(sizeof(rhs_values) / sizeof(rhs_values[0])));
}

/* ------------------------------------------------------------------
 * 5. Random fuzz across the FULL (lhs, rhs) space - genuine 4-billion
 *    pair exhaustiveness is impractical even under emulation, so this
 *    substitutes broad random coverage of both operands simultaneously,
 *    cross-checked against the exact-integer oracle with zero tolerance.
 * ------------------------------------------------------------------ */
static void test_random_fuzz(void)
{
    const int N = 200000;
    int i;

    srand(1); /* fixed seed - reproducible across runs and machines */

    for (i = 0; i < N; i++) {
        fxp16_t lhs = (fxp16_t)((rand() << 1) ^ rand());
        fxp16_t rhs = (fxp16_t)((rand() << 1) ^ rand());

        fxp16_t expected = fxp_mul_reference(lhs, rhs);
        fxp16_t actual   = fxp_mul(lhs, rhs);
        assert(actual == expected);
    }

    printf("test_random_fuzz: PASS (%d random pairs)\n", N);
}

/* ------------------------------------------------------------------
 * 6. FXP_MIN negation hazard, documented explicitly. This is the
 *    accepted trade-off for choosing -32768 (true minimum) over a
 *    symmetric -32767: negating FXP_MIN via multiply-by-(-1.0) does
 *    NOT produce +512.0 (unrepresentable in 10:6 anyway - max is
 *    +511.984375), and does NOT cleanly saturate to FXP_MAX either.
 *    It wraps back to FXP_MIN itself - the standard two's complement
 *    INT_MIN hazard, same as C's -(INT_MIN) being undefined behaviour.
 *
 *    Traced through fxp_mul: lhs=FXP_MIN(-32768), rhs=-64(-1.0).
 *    product = -32768 * -64 = 2097152, hi = 32 -> hi > 31, so this
 *    case actually DOES hit the +INF clamp path and correctly
 *    saturates to FXP_MAX. This is worth confirming explicitly rather
 *    than assuming the hazard applies here: multiplying by -1.0 goes
 *    through the same overflow check as any other multiply, and
 *    2097152 does exceed the safe threshold, so the clamp saves this
 *    particular case. The genuine hazard is at the C/scalar level -
 *    -FXP_MIN as a bare negation, outside fxp_mul entirely - which
 *    this test also checks directly.
 * ------------------------------------------------------------------ */
static void test_fxp_min_negation_hazard(void)
{
    /* multiplying FXP_MIN by -1.0 (raw -64) goes through fxp_mul's
       own overflow check like any other product, and correctly
       clamps to FXP_MAX rather than wrapping - the clamp path
       protects this specific case */
    assert(fxp_mul(FXP_MIN, -64) == FXP_MAX);

    /* the actual hazard: a bare C-level negation of FXP_MIN, with no
       clamp logic involved at all, wraps back to FXP_MIN itself
       rather than producing +32768 (which wouldn't fit anyway).
       This mirrors -(INT_MIN) being undefined behaviour in C, and is
       the accepted, documented cost of choosing -32768 over -32767. */
    assert((fxp16_t)(-FXP_MIN) == FXP_MIN);

    printf("test_fxp_min_negation_hazard: PASS "
           "(fxp_mul clamps correctly; bare negation wraps as documented)\n");
}

static void test_fxp_mul() {
    test_known_examples();
    test_overflow_boundary_exact();
    test_overflow_gross();
    test_exhaustive_by_lhs();
    test_random_fuzz();
    test_fxp_min_negation_hazard();

    printf("All fxp_mul tests passed.\n");
}


#endif
