#ifndef TEST_SIM_H
#define TEST_SIM_H

#include <stdio.h>

#include "../FXP/VEC/fxp_vec2D_types.h"
#include "../FXP/VEC/fxp_vec2D_conversions.h"

#include "../FXP/fxp_conversions.h"
#include "../FXP/fxp_rectangle.h"

#include "../SIM/sim_kinetic.h"

void test_sim() {

    printf("test sim\n");

    fxp_rectangle_t r = fxp_rectangle_make(0, 0, fxp_fix_int(320), fxp_fix_int(200));
    printf("%s\n", fxp_rectangle_str(r));
    printf("%lX\n", r.pairs[0].u);
    printf("%X\n", r.pairs[0].i);
    printf("%X\n", r.pairs[0].j);
    printf("%lX\n", r.pairs[1].u);
    printf("%X\n", r.pairs[1].i);
    printf("%X\n", r.pairs[1].j);
    printf("%llX\n", r.u);

    fxp_rectangle_t c;
    c.u = 0x3200500000000000;
    printf("%s\n", fxp_rectangle_str(c));


    sim_kinetic_t k = sim_kinetic_make(
        fxp_vec2D_null,
        fxp_vec2D_fix_float(1.0f, 0.5f),
        fxp_vec2D_null
    );

    printf("%s\n", sim_kinetic_str(&k));
}

#endif
