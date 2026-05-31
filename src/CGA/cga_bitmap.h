/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 *
 * Both CGA graphics modes (hi and lo res) use the same 16K VRAM framebuffer
 * and the VRAM layout that stores even and odd scanlines in two separate buffers.
 */
#ifndef CGA_BITMAP_H
#define CGA_BITMAP_H

#include "cga_types.h"
#include "../MEM/dos_mem_arena.h"

#include <stddef.h>
#include <stdio.h>

#define BMP_DEPTH   0
#define BMP_WIDTH   2
#define BMP_HEIGHT  4
#define BMP_SIZE    6
#define BMP_BLOCKS  8
#define BMP_DATA    10

#pragma pack(1)
typedef struct {
    cga_colour_depth_t depth;   // 0    W   mode 6 (hi res) = 1, mode 4 (lo res) = 2
    cga_coord_t width;          // 2    W   mode 6 = 0..639, mode 4 = 0..319
    cga_coord_t height;         // 4    W   modes 4 and 6 = 0..199
    cga_size_t size;            // 6    W   data block size bytes max 64K
    cga_size_t blocks;          // 8    W   number of shift optimised data blocks 0, 3 lo res 7 hi res
    char* data[8];              // 10   DW  pointer(s) 1 data block solitary image, 4 or 8 data blocks for pre shifted images
} cga_bitmap_t;
#pragma pack()

cga_bitmap_t* cga_make_bmp(cga_bitmap_t* bmp, cga_colour_depth_t depth, cga_coord_t width, cga_coord_t height);

cga_bitmap_t* cga_bmp_load(const char* file_path, mem_arena_t* arena);

dos_memsize_t cga_bmp_save(const char* file_path, const cga_bitmap_t* bmp);

void cga_bmp_dump(FILE* f, cga_bitmap_t* bmp);

#endif
