#ifndef CGA_COLOURS_H
#define CGA_COLOURS_H

#include "cga_types.h"

#define RGB_BLACK       0x000000
#define RGB_GREEN       0x00AA00
#define RGB_CYAN        0x00AAAA
#define RGB_DK_GRAY     0x555555
#define RGB_LT_BLUE     0x5555FF
#define RGB_LT_GREEN    0x55FF55
#define RGB_LT_CYAN     0x55FFFF
#define RGB_RED         0xAA0000
#define RGB_MAGENTA     0xAA00AA
#define RGB_BROWN       0xAA5500
#define RGB_LT_GRAY     0xAAAAAA
#define RGB_LT_RED      0xFF5555
#define RGB_LT_MAGENTA  0xFF55FF
#define RGB_YELLOW      0xFFFF55
#define RGB_WHITE       0xFFFFFF

typedef enum {
    CGA_PALETTE_0 = 0,
    CGA_PALETTE_0_HI,
    CGA_PALETTE_1,
    CGA_PALETTE_1_HI,
    CGA_PALETTE_2,
    CGA_PALETTE_2_HI
} cga_palette_number_t;

typedef enum {
    CGA_HI_RES_BLACK = 0,
    CGA_HI_RES_WHITE = 0x80     // 1000 0000b performance optimisation for easier shr
} cga_hi_res_colour_t;

typedef enum {
    CGA_LO_RES_BLACK        = 0x00,     // 0000 0000
    // Palette 0
	CGA_LO_RES_GREEN        = 0x40,
	CGA_LO_RES_RED          = 0x80,
	CGA_LO_RES_BROWN        = 0xC0,
	// Palette 0 High Intensity
	CGA_LO_RES_LT_GREEN     = 0x40,
	CGA_LO_RES_LT_RED       = 0x80,
	CGA_LO_RES_YELLOW       = 0xC0,
    // Palette 1
	CGA_LO_RES_CYAN         = 0x40,     // 0100 0000
	CGA_LO_RES_MAGENTA      = 0x80,     // 1000 0000
	CGA_LO_RES_LT_GRAY      = 0xC0,     // 1100 0000
	// Palette 1 High Intensity
	CGA_LO_RES_LT_CYAN      = 0x40,
	CGA_LO_RES_LT_MAGENTA   = 0x80,
	CGA_LO_RES_WHITE        = 0xC0,
} cga_lo_res_colour_t;

typedef enum {
    CGA_BLACK = 0,  // 0000
    CGA_BLUE,       // 0001
    CGA_GREEN,      // 0010
    CGA_CYAN,       // 0011
    CGA_RED,        // 0100
    CGA_MAGENTA,    // 0101
    CGA_BROWN,      // 0110
    CGA_LTGRAY,     // 0111 (IBM: "dark white")
    CGA_DKGRAY,     // 1000 (IBM: "light black")
    CGA_LTBLUE,     // 1001
    CGA_LTGREEN,    // 1010
    CGA_LTCYAN,     // 1011
    CGA_LTRED,      // 1100
    CGA_LTMAGENTA,  // 1101
    CGA_YELLOW,     // 1110 (high-intensity brown)
    CGA_WHITE       // 1111
} cga_pallete_colour_t;

/**
*  @note in 640x200 monochrome mode 6 the default colour defaults to white but can be selected from any of the 16 colours in the palette
*
*  @note In 320x200 graphics mode, the background & border colour can be changed from black (default) to any of the other 15 colors of the CGA palette.
*  This allows for some variation, as well as flashing effects, as the background color can be changed without having to redraw the screen (i.e. without changing the contents of the video RAM.)
*		Palette 0				        Palette 0 high intensity
*	#	palette colour	    RGB         palette colour          RGB
*	0		    default						    default
*	1		2	green		#00AA00		10	    light green     #55FF55
*	2		4	red			#AA0000		12	    light red	    #FF5555
*	3		6	brown		#AA5500		14	    yellow		    #FFFF55
*
*		Palette 1				        Palette 1 high intensity
*	#   palette colour      RGB         palette colour          RGB
*	0	        default				            default
*	1		3	cyan        #00AAAA	    11		light cyan	    #55FFFF
*	2		5	magenta	    #AA00AA	    13		light magenta   #FF55FF
*	3		7	light gray	#AAAAAA	    15		white		    #FFFFFF
*
*  @note A third 320x200 4-color palette is achieved by disabling the composite color burst bit while in graphics mode.
*  This is what IBM BIOS Mode 5 does
*		Palette 2				        Palette 2 high intensity
*	#	palette colour    RGB		    palette colour		    RGB
*	0		default				                default
*	1		3	cyan        #00AAAA     11		light cyan	    #55FFFF
*	2		4	red         #AA0000	    13		light red	    #FF5555
*	3		7	light gray  #AAAAAA     15		white		    #FFFFFF
*/
static const cga_argb_t cga_rgb_palette[16] = {
  0x000000,    // 0  "default" colour aka black
  0x00AA00,	   // 1  green
  0x00AAAA,    // 3	 cyan
  0xAA0000,    // 4	 red
  0xAA00AA,    // 5	 magenta
  0xAA5500,    // 6	 brown
  0xAAAAAA,    // 7	 light gray
  0x555555,    // 8  dark gray
  0x5555FF,    // 9  light blue
  0x55FF55,    // 10 light green
  0x55FFFF,    // 11 light cyan
  0xFF5555,    // 12 light red
  0xFF55FF,    // 13 light magenta
  0xFFFF55,    // 14 yellow
  0xFFFFFF,    // 15 white
};

#endif
