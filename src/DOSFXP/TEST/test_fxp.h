#ifndef TEST_FXP_H
#define TEST_FXP_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "../FXP/fxp_types.h"
#include "../FXP/fxp_conversions.h"
#include "../FXP/fxp_constants.h"
#include "../FXP/fxp_limits.h"
#include "../FXP/fxp_parts.h"

static void test_exhaustive_roundtrip() {
    int32_t raw;

    for (raw = INT16_MIN; raw <= INT16_MAX; raw++) {
        fxp16_t x = (fxp16_t)raw;
        float   f = fxp_unfix_float(x);
        fxp16_t y = fxp_fix_float(f);

        assert(x == y);
    }

    printf("test_exhaustive_roundtrip: PASS (65536/65536 values)\n");
}

static void test_known_constants() {
    static const float cases[] = {
        0.0f, 1.0f, -1.0f, 0.5f, -0.5f,
        3.14159265f, -3.14159265f,
        511.984375f,   /* == FXP_MAX exactly */
        -512.0f        /* == FXP_MIN exactly */
    };
    const float half_lsb = 0.5f / (float)FXP_ONE;
    size_t i;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        float original  = cases[i];
        fxp16_t fixed    = fxp_fix_float(original);
        float recovered  = fxp_unfix_float(fixed);
        float error      = recovered - original;

        if (error < 0.0f) error = -error;
        assert(error <= half_lsb);
    }

    /* exact boundary checks - these constants are exact multiples of
       1/64, so the round trip should hit the limit values precisely,
       not just "within tolerance" */
    assert(fxp_fix_float(511.984375f) == FXP_MAX);
    assert(fxp_fix_float(-512.0f)     == FXP_MIN);

    printf("test_known_constants: PASS\n");
}

/* ------------------------------------------------------------------
 * 1. In-range values pass through unchanged, including the exact
 *    boundary values themselves - the comparison is strict (> / <),
 *    so FXP_MAX and FXP_MIN must NOT be clamped, only exceeded.
 * ------------------------------------------------------------------ */
static void test_in_range_passthrough(void)
{
    static const int32_t cases[] = {
        0, 1, -1, 100, -100,
        FXP_MAX,   /* 32767 - exactly the boundary, must pass through */
        FXP_MIN    /* -32768 - exactly the boundary, must pass through */
    };
    size_t i;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        fxp16_t result = fxp_saturate(cases[i]);
        assert((int32_t)result == cases[i]);
    }

    printf("test_in_range_passthrough: PASS\n");
}

/* ------------------------------------------------------------------
 * 2. Values one step beyond the boundary must clamp, not wrap.
 *    This is the case a naive (fxp16_t)n cast would get wrong -
 *    FXP_MAX + 1 as a raw cast would wrap to FXP_MIN, not clamp to
 *    FXP_MAX, so this is the specific bug fxp_saturate exists to
 *    prevent.
 * ------------------------------------------------------------------ */
static void test_boundary_clamping(void)
{
    assert(fxp_saturate((int32_t)FXP_MAX + 1) == FXP_MAX);
    assert(fxp_saturate((int32_t)FXP_MIN - 1) == FXP_MIN);

    printf("test_boundary_clamping: PASS\n");
}

/* ------------------------------------------------------------------
 * 3. Extreme int32_t values must clamp, not undefined-behaviour their
 *    way into garbage. This is the realistic worst case for a wide
 *    multiply result gone wrong (e.g. a runaway velocity accumulator).
 * ------------------------------------------------------------------ */
static void test_extreme_values(void)
{
    assert(fxp_saturate(INT32_MAX) == FXP_MAX);
    assert(fxp_saturate(INT32_MIN) == FXP_MIN);

    printf("test_extreme_values: PASS\n");
}

/* ------------------------------------------------------------------
 * 4. Swept coverage either side of both boundaries, ±1000 in steps of
 *    1, to catch any off-by-one in the comparison operators
 *    themselves (e.g. accidentally using >= instead of >).
 * ------------------------------------------------------------------ */
static void test_boundary_sweep(void)
{
    int32_t offset;

    for (offset = -1000; offset <= 1000; offset++) {
        int32_t n = (int32_t)FXP_MAX + offset;
        fxp16_t result = fxp_saturate(n);

        if (n > FXP_MAX) {
            assert(result == FXP_MAX);
        } else if (n < FXP_MIN) {
            assert(result == FXP_MIN);
        } else {
            assert((int32_t)result == n);
        }
    }

    for (offset = -1000; offset <= 1000; offset++) {
        int32_t n = (int32_t)FXP_MIN + offset;
        fxp16_t result = fxp_saturate(n);

        if (n > FXP_MAX) {
            assert(result == FXP_MAX);
        } else if (n < FXP_MIN) {
            assert(result == FXP_MIN);
        } else {
            assert((int32_t)result == n);
        }
    }

    printf("test_boundary_sweep: PASS (4002 values checked)\n");
}

/* ------------------------------------------------------------------
 * 5. Random coverage across the full int32_t range, for values with
 *    no particular reason to sit near a boundary - a broad sanity net
 *    on top of the targeted tests above.
 * ------------------------------------------------------------------ */
static void test_random_wide_range(void)
{
    const int N = 100000;
    int i;

    srand(1); /* fixed seed - reproducible across runs and machines */

    for (i = 0; i < N; i++) {
        int32_t n = ((int32_t)rand() << 16) ^ (int32_t)rand();
        fxp16_t result = fxp_saturate(n);

        if (n > FXP_MAX) {
            assert(result == FXP_MAX);
        } else if (n < FXP_MIN) {
            assert(result == FXP_MIN);
        } else {
            assert((int32_t)result == n);
        }
    }

    printf("test_random_wide_range: PASS (%d values checked)\n", N);
}

static void test_saturate() {
    test_in_range_passthrough();
        test_boundary_clamping();
        test_extreme_values();
        test_boundary_sweep();
        test_random_wide_range();

        printf("All fxp_saturate tests passed.\n");
}

/* ------------------------------------------------------------------
 * 1. Known hand-worked examples, one per function, chosen to expose
 *    disagreement between trunc/floor/ceil/round on the same inputs -
 *    these are the values used to originally define the four
 *    semantics during design, kept here as a fixed reference set.
 * ------------------------------------------------------------------ */
static void test_known_examples(void)
{
    /* raw, expected_trunc, expected_floor, expected_ceil, expected_round */
    struct { fxp16_t raw; int16_t t, f, c, r; } cases[] = {
        { -112, -1, -2, -1, -2 },  /* -1.75 */
        {  -80, -1, -2, -1, -1 },  /* -1.25 */
        {  -32,  0, -1,  0,  0 },  /* -0.5  - round ties toward +inf */
        {   80,  1,  1,  2,  1 },  /*  1.25 */
        {  112,  1,  1,  2,  2 },  /*  1.75 */
        {  128,  2,  2,  2,  2 },  /*  2.0 exact - all four agree */
        {    0,  0,  0,  0,  0 }   /*  0.0 exact */
    };

    for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        assert(fxp_unfix_truncate(cases[i].raw) == cases[i].t);
        assert(fxp_unfix_floor(cases[i].raw)    == cases[i].f);
        printf("fxp_unfix_ceiling(cases[%i].raw) %i == cases[%i].c %i\n", i, fxp_unfix_ceiling(cases[i].raw), i, cases[i].c);
        assert(fxp_unfix_ceiling(cases[i].raw)     == cases[i].c);
        assert(fxp_unfix_round(cases[i].raw)    == cases[i].r);
    }

    printf("test_known_examples: PASS\n");
}

/* ------------------------------------------------------------------
 * 2. Exhaustive check of fxp_unfix_floor across all 65536 values,
 *    against an independent oracle (double division + libm floor),
 *    not a re-derivation of the same shift the function itself uses.
 * ------------------------------------------------------------------ */
static void test_exhaustive_floor(void)
{
    int32_t raw;

    for (raw = INT16_MIN; raw <= INT16_MAX; raw++) {
        fxp16_t x = (fxp16_t)raw;
        int16_t expected = (int16_t)floor((double)x / (double)FXP_ONE);
        assert(fxp_unfix_floor(x) == expected);
    }

    printf("test_exhaustive_floor: PASS (65536/65536 values)\n");
}

/* ------------------------------------------------------------------
 * 3. Exhaustive check of fxp_unfix_truncate. Conversion from double
 *    to integer truncates toward zero per the C standard, so a plain
 *    cast is a valid independent oracle here.
 * ------------------------------------------------------------------ */
static void test_exhaustive_truncate(void)
{
    int32_t raw;

    for (raw = INT16_MIN; raw <= INT16_MAX; raw++) {
        fxp16_t x = (fxp16_t)raw;
        int16_t expected = (int16_t)((double)x / (double)FXP_ONE);
        assert(fxp_unfix_truncate(x) == expected);
    }

    printf("test_exhaustive_truncate: PASS (65536/65536 values)\n");
}

/* ------------------------------------------------------------------
 * 4. Exhaustive check of fxp_unfix_ceil, against libm ceil.
 * ------------------------------------------------------------------ */
static void test_exhaustive_ceil(void)
{
    int32_t raw;

    for (raw = INT16_MIN; raw <= INT16_MAX; raw++) {
        fxp16_t x = (fxp16_t)raw;
        int16_t expected = (int16_t)ceil((double)x / (double)FXP_ONE);
        assert(fxp_unfix_ceiling(x) == expected);
    }

    printf("test_exhaustive_ceil: PASS (65536/65536 values)\n");
}

/* ------------------------------------------------------------------
 * 5. Exhaustive check of fxp_unfix_round against its OWN documented
 *    contract: add-32-then-arithmetic-shift, including the 16-bit
 *    wraparound that occurs for raw values >= 32736 (the top 32
 *    values, i.e. the last 0.5 units of range). This deliberately
 *    does NOT use true nearest-rounding as the oracle for that range,
 *    because the function was explicitly designed "fast and UB" at
 *    that edge rather than paying for a saturation check - see
 *    test_round_wraparound_edge() below for the specific boundary.
 * ------------------------------------------------------------------ */
static int16_t round_reference(fxp16_t x)
{
    int32_t added = (int32_t)x + (FXP_ONE >> 1);
    int16_t wrapped = (int16_t)added;  /* mirrors hardware ADD AX,32 wrap */
    return (int16_t)(wrapped >> FXP_FRACTIONAL_BITS);
}

static void test_exhaustive_round(void)
{
    int32_t raw;

    for (raw = INT16_MIN; raw <= INT16_MAX; raw++) {
        fxp16_t x = (fxp16_t)raw;
        int16_t expected = round_reference(x);
        assert(fxp_unfix_round(x) == expected);
    }

    printf("test_exhaustive_round: PASS (65536/65536 values, "
           "including documented wraparound region)\n");
}

/* ------------------------------------------------------------------
 * 6. Explicit, named test of the accepted round() wraparound window,
 *    so this behaviour is pinned down by a test with its own name
 *    rather than only being implicitly covered by test 5 above.
 *    Values FXP_MAX-31 .. FXP_MAX round correctly UP TO the point
 *    where the +32 bias itself overflows int16_t; at and beyond that
 *    point the result wraps to a large negative number rather than
 *    saturating. This is intentional - see fxp_unfix_round's header
 *    comment and the project's own "fast and UB" design decision.
 * ------------------------------------------------------------------ */
static void test_round_wraparound_edge(void)
{
    /* last value that rounds correctly without wrapping */
    assert(fxp_unfix_round((fxp16_t)(FXP_MAX - 32)) == 511);

    /* first value where the +32 bias overflows int16_t and wraps */
    assert(fxp_unfix_round((fxp16_t)(FXP_MAX - 31)) < 0);

    /* FXP_MAX itself is inside the wrapped region */
    assert(fxp_unfix_round(FXP_MAX) < 0);

    printf("test_round_wraparound_edge: PASS "
           "(confirmed wrap begins at FXP_MAX-31)\n");
}

static void test_unfix() {
    test_known_examples();
    test_exhaustive_floor();
    test_exhaustive_truncate();
    test_exhaustive_ceil();
    test_exhaustive_round();
    test_round_wraparound_edge();

    printf("All fxp_unfix tests passed.\n");
}

static void test_exhaustive_parts() {
    int32_t raw;

    for (raw = INT16_MIN; raw <= INT16_MAX; raw++) {
        fxp16_t v     = (fxp16_t)raw;
        fxp16_t whole = fxp_part_whole(v);
        fxp16_t frac  = fxp_part_frac(v);

        /* reconstruction: the two parts must sum back to the original */
        assert((int16_t)(whole + frac) == v);

        /* frac must always be non-negative (0..63), regardless of sign -
           it's a remainder after flooring, not a signed distance */
        assert(frac >= 0 && frac <= FXP_PART_FRAC_MASK);

        /* whole must always be a multiple of FXP_ONE (fraction bits
           clear), and must equal floor(v) scaled back up by FXP_ONE -
           checked against fxp_unfix_floor as an independent oracle */
        assert((whole & FXP_PART_FRAC_MASK) == 0);
        assert((int16_t)(whole >> FXP_FRACTIONAL_BITS) == fxp_unfix_floor(v));
    }

    printf("test_exhaustive_parts: PASS (65536/65536 values)\n");
}

static void test_fxp() {
    printf("Test DOSFXP...\n");
    test_exhaustive_roundtrip();
    test_known_constants();
    test_saturate();
    test_unfix();
    test_exhaustive_parts();
}

#endif
