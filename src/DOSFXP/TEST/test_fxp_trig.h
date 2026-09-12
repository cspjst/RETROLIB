#include "../FXP/fxp_trigonometry.h"

#include "../FXP/fxp_types.h"
#include "../FXP/fxp_conversions.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>


/*
// 2PI radians / 256 8bit steps i.e. 0xFF
// 6.28318531 / 256
static const double radian_step = 0.02454369;

void fxp_build_table() {
    printf("BAM8 Sine Table[256]\n");
    for(int i = 0; i < 257; ++i) {
        double a = sin(radian_step * (double)i);
        printf("%3d, ", fxp_fix_float(a));
    }
}
*/


/* Table was interpolated from a coarser 256-entry BAM8 source, so
   allow a small tolerance rather than demanding bit-exact match
   against a freshly-computed double-precision sine. */
#define TABLE_TOLERANCE 2

static int close_enough(fxp16_t got, double true_deg)
{
    int expected = (int)(sin(true_deg * 3.14159265358979323846 / 180.0) * 64.0 + (sin(true_deg*3.14159265358979323846/180.0)>=0 ? 0.5 : -0.5));
    int diff = got - expected;
    if (diff < 0) diff = -diff;
    return diff <= TABLE_TOLERANCE;
}

/* ------------------------------------------------------------------
 * 1. The exact angles from the original eyeball test, now with real
 *    assertions instead of a human reading printf output.
 * ------------------------------------------------------------------ */
static void test_known_quadrants(void)
{
    static const int angles[] = { 0, 90, 180, 270, 360, 450, -90 };
    size_t i;

    for (i = 0; i < sizeof(angles)/sizeof(angles[0]); i++) {
        fxp16_t a = fxp_fix_int(angles[i]);
        fxp16_t result = fxp_sin(a);
        assert(close_enough(result, (double)angles[i]));
    }

    printf("test_known_quadrants: PASS\n");
}

/* ------------------------------------------------------------------
 * 2. Every integer degree 0..359 - full single-revolution coverage.
 * ------------------------------------------------------------------ */
static void test_full_revolution(void)
{
    int deg;

    for (deg = 0; deg < 360; deg++) {
        fxp16_t a = fxp_fix_int(deg);
        fxp16_t result = fxp_sin(a);
        assert(close_enough(result, (double)deg));
    }

    printf("test_full_revolution: PASS (0-359 degrees)\n");
}

/* ------------------------------------------------------------------
 * 3. Wide sweep within fxp16_t's ACTUAL representable range as
 *    degrees. fxp16_t's whole part only spans roughly -512..+511, so
 *    "degrees" input can represent at most about 1.4 revolutions
 *    either way - NOT the multiple full revolutions a first attempt
 *    at this test assumed (which silently overflowed int16_t and
 *    produced meaningless garbage, not a real fxp_sin bug - a
 *    reminder that fxp_fix_int(1080) is itself invalid input, same
 *    class of hazard as everything else this library has had to
 *    guard against).
 * ------------------------------------------------------------------ */
static void test_wide_sweep(void)
{
    int deg;

    for (deg = -511; deg <= 511; deg += 3) {
        fxp16_t a = fxp_fix_int(deg);
        fxp16_t result = fxp_sin(a);
        double true_deg = deg % 360;
        assert(close_enough(result, true_deg));
    }

    printf("test_wide_sweep: PASS (-511 to +511 degrees - fxp16_t's actual representable range)\n");
}

static void test_fxp_quick() {
    //fxp_build_table();
    int i[] = {0, 90, 180, 270, 360, 450, -90};
    for(int j = 0; j < 7; ++j) {
        fxp16_t a = fxp_fix_int(i[j]);
        printf("sin(%i) = %f\n", i[j], fxp_unfix_float(fxp_sin(a)));
    }
}

static void test_fxp_trig() {
    test_known_quadrants();
    test_full_revolution();
    test_wide_sweep();

    printf("All fxp_sin tests passed.\n");
}
