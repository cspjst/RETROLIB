/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 *
 * Both CGA graphics modes (hi and lo res) use the same 16K VRAM framebuffer
 * and layout that stores even and odd scanlines in two separate buffers.
 */
#ifndef CGA_HI_RES_BITMAP_H
#define CGA_HI_RES_BITMAP_H

#include "cga_types.h"
#include "cga_bitmap_constants.h"
#include "../../doslib/src/DOS/dos_memory_types.h"

#include <stddef.h>
#include <stdio.h>

typedef struct {
    cga_colour_depth_t depth;   // mode 6 (hi res) = 1, mode 4 (lo res) = 2
    cga_coord_t width;          // mode 6 = 0..639, mode 4 = 0..319
    cga_coord_t height;         // modes 4 and 6 = 0..199
    size_t size;                // data size bytes max 64K
    unsigned short palette;     // mode 6 = forground colour, mode 4 = palette 0,1,2
    char* data;                 // limited to 64K max
} cga_bitmap_t;

cga_bitmap_t* cga_make_bmp(cga_bitmap_t* bmp, cga_colour_depth_t depth, cga_coord_t width, cga_coord_t height, unsigned int pal);

/**
 * The portable bitmap format (PBM) as the raw byte format is ideal for mode 6 monochrome
 */
FILE* cga_bmp_read_meta_raw_pbm(FILE* f, cga_bitmap_t* bmp);
FILE* cga_bmp_write_meta_raw_pbm(FILE* f, cga_bitmap_t* bmp);
dos_memsize_t cga_bmp_load_raw_pbm(FILE* f, cga_bitmap_t* bmp);
dos_memsize_t cga_bmp_save_raw_pbm(FILE* f, cga_bitmap_t* bmp);

/**
 * The portable pixmap format (PPM) as the raw byte format requires more processing for mode 4/5
 */
FILE* cga_bmp_read_meta_raw_ppm(FILE* f, cga_bitmap_t* bmp);


void cga_bmp_dump(FILE* f, cga_bitmap_t* bmp);

#endif
