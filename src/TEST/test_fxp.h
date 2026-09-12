#ifndef TEST_FXP_H
#define TEST_FXP_H

#include <stdio.h>
#include "../ENV/env_video_mode.h"
#include "../CGA/cga_types.h"
#include "../CGA/LO/cga_lo_plot.h"
#include "../CGA/cga_colours.h"

void test_fxp_plot() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);
    printf("CGA 320x200 4 colour mode\n");

    cga_colour_t c = CGA_LO_RES_LT_MAGENTA;
    cga_point_t p;
    p.x = 160;
    p.y = 100;


    cga_lo_plot(p, c);

    getchar();
    env_set_video_mode(m);
}

void test_fxp() {
    test_fxp_plot();
}

#include <assert.h>
#include <stdio.h>

#endif
