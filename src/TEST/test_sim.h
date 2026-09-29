#ifndef TEST_SIM_H
#define TEST_SIM_H

#include <stdio.h>

#include "../SIM/sim_kinetic.h"
#include "../FXP/fxp_conversions.h"
#include "../FXP/VEC/fxp_vec2D_types.h"
#include "../FXP/VEC/fxp_vec2D_conversions.h"

void test_sim() {

    printf("test sim\n");

    sim_kinetic_t k = sim_kinetic_make(
        fxp_vec2D_null,
        fxp_vec2D_fix_float(1.0f, 0.5f),
        fxp_vec2D_null
    );
}

#endif
