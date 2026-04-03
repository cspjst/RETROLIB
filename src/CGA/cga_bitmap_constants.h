#ifndef CGA_BITMAP_CONSTANTS_H
#define CGA_BITMAP_CONSTANTS_H

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
