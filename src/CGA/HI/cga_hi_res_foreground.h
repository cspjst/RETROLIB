#ifndef CGA_HI_RES_FOREGROUND_H
#define CGA_HI_RES_FOREGROUND_H

#include "../cga_colours.h"

/**
 * 03D9h: Colour control register Mode 6 "hi res" 640x200 mode
 *
 * Bits 3-0: select foreground colour.
 * bit 3 = Intensity, Bit 2 = Red, Bit 1 = Green, Bit 0 = Blue
 */

void cga_hi_res_set_fg(cga_pallete_colour_t c);


#endif
