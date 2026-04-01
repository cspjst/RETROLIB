#ifndef CGA_HI_RES_PLOT_H
#define CGA_HI_RES_PLOT_H

#include "cga_types.h"

#define cga_plot(x, y, c) cga_hi_res_plot_lookup(x, y, c)

void cga_hi_res_plot_calculate(cga_coord_t x, cga_coord_t y, cga_colour_t colour);

void __fastcall cga_hi_res_plot_lookup(cga_coord_t x, cga_coord_t y, cga_colour_t colour);

#endif
