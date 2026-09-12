#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "../FXP/fxp_types.h"
#include "../FXP/fxp_constants.h"
#include "../FXP/fxp_operators.h"

static fxp16_t ref(fxp16_t x) {
    return (fxp16_t)floor(sqrt((double)x * FXP_ONE));
}

static void test_fxp_sqrt() {
    printf("test exhaustive - very slow on XT or emulated XT...\n");

    int32_t x;

    assert(fxp_sqrt(0)   == 0);
    assert(fxp_sqrt(64)  == 64);    /* sqrt(1.0)  = 1.0 */
    assert(fxp_sqrt(256) == 128);   /* sqrt(4.0)  = 2.0 */

    for (x = 0; x <= INT16_MAX; x++) {
        assert(fxp_sqrt((fxp16_t)x) == ref((fxp16_t)x));
    }

    printf("fxp_sqrt: PASS (32768/32768 values)\n");
}
