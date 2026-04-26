/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 *
 * Both CGA graphics modes (hi and lo res) use the same 16K VRAM framebuffer
 * and layout that stores even and odd scanlines in two separate buffers.
 */
#ifndef CGA_BITMAP_H
#define CGA_BITMAP_H

#include "cga_types.h"
#include "../MEM/dos_mem_arena.h"

#include <stddef.h>
#include <stdio.h>

typedef struct {
    cga_colour_depth_t depth;   // mode 6 (hi res) = 1, mode 4 (lo res) = 2
    cga_coord_t width;          // mode 6 = 0..639, mode 4 = 0..319
    cga_coord_t height;         // modes 4 and 6 = 0..199
    cga_size_t palette;         // mode 6 = forground colour, mode 4 = palette 0,1,2
    cga_size_t size;            // data block size bytes max 64K
    cga_size_t shifts;          // number of shift optimised data blocks 0, 3 lo res 7 hi res
    char* data[8];              // pointers 1 or 8 data blocks for pre shifted images
} cga_bitmap_t;

cga_bitmap_t* cga_make_bmp(cga_bitmap_t* bmp, cga_colour_depth_t depth, cga_coord_t width, cga_coord_t height, unsigned int pal);

/**
 * Load native format .cga file data into a cga bitmap
 */
cga_bitmap_t* cga_bmp_load(const char* file_path, mem_arena_t* arena);

/**
 * Save a cga bitmap in native format as a .cga file
 */
dos_memsize_t cga_bmp_save(const char* file_path, const cga_bitmap_t* bmp);

void cga_bmp_dump(FILE* f, cga_bitmap_t* bmp);

#endif
