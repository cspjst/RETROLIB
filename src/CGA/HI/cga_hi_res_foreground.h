#ifndef CGA_HI_RES_FOREGROUND_H
#define CGA_HI_RES_FOREGROUND_H

#include "../cga_colours.h"

/**
* In mode 6 640x200 CGA "hi res" the foreground colour can be
* set to a 4 bit value for 1 of the 16 colours:
*  BLACK           // 0000
*  BLUE            // 0001
*  GREEN           // 0010
*  CYAN            // 0011
*  RED             // 0100
*  MAGENTA         // 0101
*  BROWN           // 0110
*  LTGRAY          // 0111 (IBM: "dark white")
*  DKGRAY          // 1000 (IBM: "light black")
*  CGA_LTBLUE      // 1001
*  CGA_LTGREEN     // 1010
*  CGA_LTCYAN      // 1011
*  CGA_LTRED       // 1100
*  CGA_LTMAGENTA   // 1101
*  CGA_YELLOW      // 1110 (high-intensity brown)
*  CGA_WHITE       // 1111
*/
void cga_hi_set_foreground_colour(cga_palette_colour_t colour);


#endif
