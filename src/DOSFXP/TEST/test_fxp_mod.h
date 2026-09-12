#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "../FXP/fxp_types.h"
#include "../FXP/fxp_constants.h"
#include "../FXP/fxp_operators.h"
#include "../FXP/fxp_int0_handler.h"
#include "../FXP/fxp_conversions.h"

static void test_fxp_mod_samples() {
    fxp_install_int0_handler();

    float a = 9.2f;
    float b = 3.7f;
    fxp16_t x = fxp_fix_float(a);
    fxp16_t y = fxp_fix_float(b);
    printf("Remainder of %.2f / %.2f is %.2f\n", a, b, fmod(a, b));
    printf("Remainder of %.2f / %.2f is %.2f\n", fxp_unfix_float(x), fxp_unfix_float(y), fxp_unfix_float(fxp_mod(x, y)));
    a = -10.5f;
    b = 3.0f;
    x = fxp_fix_float(a);
    y = fxp_fix_float(b);
    printf("Remainder of %.2f / %.2f is %.2f\n", a, b, fmod(a, b));
    printf("Remainder of %.2f / %.2f is %.2f\n", fxp_unfix_float(x), fxp_unfix_float(y), fxp_unfix_float(fxp_mod(x, y)));
    a = 5.0f;
    b = 0.0f;
    x = fxp_fix_float(a);
    y = fxp_fix_float(b);
    printf("Remainder of %.2f / %.2f is %.2f\n", a, b, fmod(a, b));
    printf("Remainder of %.2f / %.2f is %.2f\n", fxp_unfix_float(x), fxp_unfix_float(y), fxp_unfix_float(fxp_mod(x, y)));
}

static void test_fxp_mod_360() {
    float b = 360.0f;
    fxp16_t y = fxp_fix_float(b);

    for(float a = -512.0f; a < 512.0f; a += 0.5f) {
        fxp16_t x = fxp_fix_float(a);
        printf("Remainder of %.2f / %.2f is %.2f\n", a, b, fmod(a, b));
        printf("Remainder of %.2f / %.2f is %.2f\n", fxp_unfix_float(x), fxp_unfix_float(y), fxp_unfix_float(fxp_mod(x, y)));
    }
}

static void test_fxp_mod() {
    test_fxp_mod_samples();
    test_fxp_mod_360();
}
