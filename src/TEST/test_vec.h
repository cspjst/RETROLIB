#ifndef TEST_VEC_H
#define TEST_VEC_H

#include <stdio.h>
#include <stdint.h>
#include "../ENV/env_video_mode.h"
#include "../CGA/cga_types.h"
#include "../CGA/LO/cga_lo_plot.h"
#include "../CGA/cga_colours.h"
//#include "../FXP/fxp_types.h"
#include "../FXP/fxp_conversions.h"
//#include "../FXP/fxp_operators.h"
//#include "../FXP/fxp_trigonometry.h"

#include "../FXP/VEC/fxp_vec2d_types.h"
#include "../FXP/VEC/fxp_vec2d_constants.h"
#include "../FXP/VEC/fxp_vec2D_conversions.h"
#include "../FXP/VEC/fxp_vec2D_operators.h"
#include "../FXP/VEC/fxp_vec2D_io.h"


void test_vec_math() {
    fxp_vector2D_t v = {0};
    fxp_vector2D_t p = {0};

    v.u = FXP_VEC2D_UNIT_I;
    printvf(v);
    v.u = FXP_VEC2D_UNIT_J;
    printvf(v);
    v.u = FXP_VEC2D_ZERO;
    printvf(v);
    v = fxp_vec2D_fix_polar(fxp_fix_int(10), 150);
    printvi(v);
    printvf(v); // should be (-8.66,5) but accuracy loss gives (-8.75,5)
    p = v;
    printvf(p);
    printvi(fxp_vec2D_unfix_round(p));

}

void test_vec_plot() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);
    printf("CGA 320x200 4 colour mode\n");

    fxp_vector2D_t v = fxp_vec2D_fix_polar(fxp_fix_int(100), 50);
    cga_lo_plot(fxp_vec2D_to_cga_point(fxp_vec2D_unfix_round(v)), CGA_LO_RES_CYAN);

    v = fxp_vec2D_scale(v, 96);
    cga_lo_plot(fxp_vec2D_to_cga_point(fxp_vec2D_unfix_round(v)), CGA_LO_RES_CYAN);

    v = fxp_vec2D_add(v, fxp_vec2D_fix_polar(fxp_fix_int(10), 25));
    cga_lo_plot(fxp_vec2D_to_cga_point(fxp_vec2D_unfix_round(v)), CGA_LO_RES_MAGENTA);

    getchar();
    env_set_video_mode(m);
}

void test_vec_spiral() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);
    printf("CGA 320x200 4 colour mode - spiral\n");

    int i;
    fxp_vector2D_t w = fxp_vec2D_fix_int(160, 100);
    for (i = 0; i < 720; ++i) {
        fxp16_t r = fxp_fix_int(i / 8);       /* radius grows 0 -> ~90px over the loop */
        int16_t theta = (int16_t)(i * 3);     /* 3 degrees/step - 6 full turns total */
        fxp_vector2D_t v = fxp_vec2D_fix_polar(r, theta);
        cga_colour_t colour = ((i / 30) & 1) ? CGA_LO_RES_MAGENTA : CGA_LO_RES_CYAN;

        cga_lo_plot(fxp_vec2D_to_cga_point(fxp_vec2D_unfix_round(fxp_vec2D_add(v, w))), colour);
    }

    getchar();
    env_set_video_mode(m);
}

void test_vec() {
    //test_vec_math();
    //test_vec_plot();
    test_vec_spiral();
}

#endif
