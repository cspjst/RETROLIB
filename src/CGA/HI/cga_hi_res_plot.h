/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#ifndef CGA_HI_RES_PLOT_H
#define CGA_HI_RES_PLOT_H

#include "../cga_types.h"
#include "cga_hi_res_colours.h"

#define cga_plot(x, y, c) cga_hi_res_plot_lookup(x, y, c)

void cga_hi_res_plot_calculate(cga_coord_t x, cga_coord_t y, cga_hi_res_colours_t colour);

void __fastcall cga_hi_res_plot_lookup(cga_coord_t x, cga_coord_t y, cga_hi_res_colours_t colour);

char __fastcall cga_hi_res_sync_plot(cga_coord_t x, cga_coord_t y, cga_hi_res_colours_t colour);

#endif
