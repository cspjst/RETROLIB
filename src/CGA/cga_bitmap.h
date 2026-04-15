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

#include <stdio.h>

typedef struct {
    cga_colour_depth_t depth;    
    cga_coord_t width;
    cga_coord_t height;
    dos_memsize_t size;
    char* data;             // limited to 64K max
} cga_bitmap_t;

cga_bitmap_t* cga_make_bmp(cga_bitmap_t* bmp, unsigned short depth, cga_coord_t width, cga_coord_t height);

/**
 * The portable bitmap format (PBM) as the raw byte format is ideal for mode 6
 */
FILE* cga_read_meta_raw_pbm(FILE* f, cga_bitmap_t* bmp);
FILE* cga_write_meta_raw_pbm(FILE* f, cga_bitmap_t* bmp);
dos_memsize_t cga_load_bmp_raw_pbm(FILE* f, cga_bitmap_t* bmp);
dos_memsize_t cga_save_bmp_raw_pbm(FILE* f, cga_bitmap_t* bmp);



#endif
