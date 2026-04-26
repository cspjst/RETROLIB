/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.5
 */
#ifndef CGA_BITMAP_CONSTANTS_H
#define CGA_BITMAP_CONSTANTS_H

#define CGA_PBM_FG "#CGA FG=%hu"        // extract pbm meta comment foreground value
#define CGA_PBM_WIDTH_HEIGHT "%hu %hu"  // extract pbm meta width and height
#define CGA_PPM_PAL "#CGA PAL=%hu"      // extract ppm meta comment palette value
#define CGA_PPM_MAXVAL "%hu"            // extract ppm meta maximum colour value
#define CGA_MAX_COLOUR 255              // byte sized

typedef enum {
    CGA_LO_RES,
    CGA_HI_RES
} cga_bmp_resolution_t;

typedef enum { // NB little endian
    CGA_PBM     = 0x3150,   // Portable BitMap      "P1"
    CGA_PGM     = 0x3250,   // Portable GrayMap     "P2"
    CGA_PPM     = 0x3350,   // Portable PixMap      "P3"
    CGA_RAW_PBM = 0x3450,   // Raw Portable BitMap  "P4"
    CGA_RAW_PGM = 0x3550,   // Raw Portable GrayMap "P5"
    CGA_RAW_PPM = 0x3650    // Raw Portable PixMap  "P6"
} cga_netpbm_t;

#endif
