#ifndef TEST_VEC_H
#define TEST_VEC_H

#include <stdio.h>
#include <stdint.h>
#include "../ENV/env_video_mode.h"
//#include "../CGA/cga_types.h"
//#include "../CGA/LO/cga_lo_plot.h"
//#include "../CGA/cga_colours.h"
//#include "../FXP/fxp_types.h"
#include "../FXP/fxp_conversions.h"
//#include "../FXP/fxp_operators.h"
//#include "../FXP/fxp_trigonometry.h"

#include "../FXP/VEC/fxp_vec2d_types.h"
#include "../FXP/VEC/fxp_vec2d_constants.h"
#include "../FXP/VEC/fxp_vec2D_conversions.h"
#include "../FXP/VEC/fxp_vec2D_io.h"


void test_vec_math() {
    fxp_vector2D_t v = {0};
    v.u = FXP_VEC2D_UNIT_I;
    printv(v);
    v.u = FXP_VEC2D_UNIT_J;
    printv(v);
    v.u = FXP_VEC2D_ZERO;
    printv(v);
    fxp_vec2D_fix_polar(&v, fxp_fix_int(10), 150);
    printv(v); // should be (-8.66,5) but accuracy loss gives (-8.75,5)

}

void test_vec_plot() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);
    printf("CGA 320x200 4 colour mode\n");


    getchar();
    env_set_video_mode(m);
}

void test_vec() {
    test_vec_math();
    //test_vec_plot();
}

#endif
