/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#ifndef CGA_LO_PLOT_H
#define CGA_LO_PLOT_H

#include "../cga_colours.h"
#include "../cga_types.h"

#define cga_plot(x, y, c) cga_lo_plot(x, y, c)

void __fastcall cga_lo_plot(cga_coord_t x, cga_coord_t y, cga_lo_res_colour_t colour);

void __fastcall cga_lo_sync_plot(cga_coord_t x, cga_coord_t y, cga_lo_res_colour_t colour);

#endif
