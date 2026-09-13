#ifndef TEST_FXP_H
#define TEST_FXP_H

#include <stdio.h>
#include <stdint.h>
#include "../ENV/env_video_mode.h"
#include "../CGA/cga_types.h"
#include "../CGA/LO/cga_lo_plot.h"
#include "../CGA/cga_colours.h"
#include "../FXP/fxp_types.h"
#include "../FXP/fxp_conversions.h"
#include "../FXP/fxp_operators.h"
#include "../FXP/fxp_trigonometry.h"


void test_fxp_plot() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);
    printf("CGA 320x200 4 colour mode\n");

    cga_colour_t c = CGA_LO_RES_LT_MAGENTA;
    cga_point_t p;
	fxp16_t r = fxp_fix_int(98);
	int cx = 160;
	int cy = 100;
	for (int j = 0; j < 25; ++j) {
	    fxp16_t a = FXP_ONE;
		for (int i = 1; i < 360; ++i) {
			p.x = cx + fxp_unfix_round(fxp_mul(r, fxp_cos(fxp_unfix_round(a))));
			p.y = cy + fxp_unfix_round(fxp_mul(r, fxp_sin(fxp_unfix_round(a))));
			cga_lo_plot(p, CGA_LO_RES_LT_CYAN);
			fxp_inc(&a);
		}
		r = fxp_sub(r, 128);

		a = FXP_ONE;
		for (int i = 1; i < 360; ++i) {
			p.x = cx + fxp_unfix_round(fxp_mul(r, fxp_cos(i)));
			p.y = cy + fxp_unfix_round(fxp_mul(r, fxp_sin(i)));
			cga_lo_plot(p, CGA_LO_RES_LT_MAGENTA);
		}
		r = fxp_sub(r, 128);
	}

    getchar();
    env_set_video_mode(m);
}

void test_fxp() {
    test_fxp_plot();
}

#endif
