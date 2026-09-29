#ifndef TEST_VEC_H
#define TEST_VEC_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "../ENV/env_video_mode.h"

#include "../CGA/cga_types.h"
#include "../CGA/LO/cga_lo_plot.h"
#include "../CGA/cga_colours.h"

//#include "../FXP/fxp_types.h"
#include "../FXP/fxp_int0_handler.h"
#include "../FXP/fxp_conversions.h"
#include "../FXP/fxp_operators.h"
#include "../FXP/fxp_trigonometry.h"

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
    cga_lo_plot(fxp_vec2D_unfix_round(v).u, CGA_LO_RES_CYAN);

    v = fxp_vec2D_scale(v, 96);
    cga_lo_plot(fxp_vec2D_unfix_round(v).u, CGA_LO_RES_CYAN);

    v = fxp_vec2D_add(v, fxp_vec2D_fix_polar(fxp_fix_int(10), 25));
    cga_lo_plot(fxp_vec2D_unfix_round(v).u, CGA_LO_RES_MAGENTA);

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

        cga_lo_plot(fxp_vec2D_unfix_round(v).u, colour);
    }

    getchar();
    env_set_video_mode(m);
}

static void test_exp_carbon14() {
    const double LN2 = 0.69314718055994530942;
    double half_lives;

    printf("\n=== Carbon-14 remaining fraction, half-lives elapsed ===\n");
    printf(" hl  |  exponent | fxp_exp  | math.h exp |   diff\n");
    printf("-----+-----------+----------+------------+--------\n");

    for (half_lives = 0.0; half_lives <= 7.001; half_lives += 0.5) {
        double x = -half_lives * LN2;
        fxp16_t x_fxp = fxp_fix_float((float)x);
        fxp16_t result_fxp = fxp_exp(x_fxp);
        double fxp_result = fxp_unfix_float(result_fxp);
        double true_result = exp(x);
        double diff = fabs(fxp_result - true_result);

        printf("%4.1f | %9.4f | %8.5f | %10.5f | %6.4f\n",
                half_lives, x, fxp_result, true_result, diff);
    }
    printf("pass but int0 should have stalled prior!\n");
}

static void test_exp_ecoli() {
    const double LN2 = 0.69314718055994530942;
    double doublings;

    printf("\n=== E. coli population multiple, doublings elapsed ===\n");
    printf("  d  |  exponent |  fxp_exp | math.h exp |   diff\n");
    printf("-----+-----------+----------+------------+--------\n");

    for (doublings = 0.0; doublings <= 9.001; doublings += 0.5) {
        double x = doublings * LN2;
        fxp16_t x_fxp = fxp_fix_float((float)x);
        fxp16_t result_fxp = fxp_exp(x_fxp);
        double fxp_result = fxp_unfix_float(result_fxp);
        double true_result = exp(x);
        double diff = fabs(fxp_result - true_result);

        printf("%4.1f | %9.4f | %8.3f | %10.3f | %6.4f\n",
               doublings, x, fxp_result, true_result, diff);
    }
    printf("pass\n");
}

static void test_graph_carbon14() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);
    printf("\n\n\n\n\n\n\n\n\n\n\n\nCGA 320x200 4 colour mode\nCarbon-14 decay\n");

    const fxp16_t NEG_LN2   = fxp_fix_float(-0.6931472f); /* -ln(2), one-off cost */
    const fxp16_t HL_STEP   = fxp_fix_float(6.9f / 280.0f);/* half-lives per pixel column */
    const fxp16_t SCALE     = fxp_fix_int(40);             /* pixels per graph unit */
    fxp_vector2D_t w        = fxp_vec2D_fix_int(20, 60);   /* screen origin: top-left of curve */

    int i;
    for (i = 0; i < 280; ++i) {
        fxp16_t hl       = fxp_mul(fxp_fix_int(i), HL_STEP);   /* 0 .. ~6.9 half-lives */
        fxp16_t exponent = fxp_mul(hl, NEG_LN2);               /* -hl*ln2 */
        fxp16_t frac     = fxp_exp(exponent);                  /* 0 .. 1, falling */

        fxp_vector2D_t v;
        v.x = hl;
        v.y = frac;    /* natural sign - the screen flip happens in the scale below */

        cga_colour_t colour = CGA_LO_RES_CYAN;

        cga_lo_plot(fxp_vec2D_unfix_round(
            fxp_vec2D_add(fxp_vec2D_scale_xy(v, SCALE, -SCALE), w)).u,
            colour
        );
    }

    getchar();
    env_set_video_mode(m);
}

static void test_graph_ecoli() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);
    printf("CGA 320x200 4 colour mode\nE.coli growth\n");

    const fxp16_t LN2       = fxp_fix_float(0.6931472f);   /* +ln(2), growth this time */
    const fxp16_t DBL_STEP  = fxp_fix_float(9.0f / 280.0f);/* doublings per pixel column */
    const fxp16_t NINE      = fxp_fix_int(9);
    const fxp16_t FIVE11    = fxp_fix_int(511);            /* true max representable int -
                                                                512 silently wraps to -512 */
    const fxp16_t SCALE     = fxp_fix_int(180);            /* pixels per graph unit */
    fxp_vector2D_t w        = fxp_vec2D_fix_int(20, 195);  /* screen origin: bottom-left of curve */

    int i;
    for (i = 0; i < 280; ++i) {
        fxp16_t doublings = fxp_mul(fxp_fix_int(i), DBL_STEP); /* 0 .. ~9 */
        fxp16_t exponent  = fxp_mul(doublings, LN2);           /* +doublings*ln2 */
        fxp16_t frac      = fxp_exp(exponent);                 /* 1 .. ~510, climbing */

        fxp_vector2D_t v;
        v.x = fxp_div(doublings, NINE);   /* normalise x to 0..1 */
        v.y = fxp_div(frac, FIVE11);      /* normalise y to 0..1, natural sign */

        cga_colour_t colour = CGA_LO_RES_MAGENTA;

        cga_lo_plot(fxp_vec2D_unfix_round(
            fxp_vec2D_add(fxp_vec2D_scale_xy(v, SCALE, -SCALE), w)).u,
            colour
        );
    }

    getchar();
    env_set_video_mode(m);
}

void test_vec() {
    fxp_install_int0_handler();
    //test_vec_math();
    //test_vec_plot();
    //test_vec_spiral();
    //test_exp_carbon14();
    //test_exp_ecoli();
    //test_graph_carbon14();
    test_graph_ecoli();
}

#endif
