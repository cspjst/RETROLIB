#ifndef CGA_LOW_PALETTE_H
#define CGA_LOW_PALETTE_H

#include "../cga_colours.h"

/**
 * number   palette             colours
 *  0        0                  default, green, red, brown
 *  1        0 high intensity   default, lt green, lt red, yellow
 *  2        1                  default, cyan, magenta, light gray
 *  3        1 high intensity   default, lt cyan, lt magenta, white
 *  4        2                  default, cyan, red, lt gray
 *  5        2 high intensity   default, lt cyan, lt red, white
 */
void cga_lo_select_palette(cga_palette_number_t pal);

#endif
