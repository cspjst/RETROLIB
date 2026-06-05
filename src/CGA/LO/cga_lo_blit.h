#ifndef CGA_LO_BLIT_H
#define CGA_LO_BLIT_H

#include "../cga_types.h"
#include "../cga_bitmap.h"

void cga_lo_blit_paste(cga_size_t x, cga_size_t y, cga_bitmap_t* bmp);

void cga_lo_blit_and_or(cga_size_t x, cga_size_t y, cga_bitmap_t* mask, cga_bitmap_t* bmp);

void cga_blit_copy(cga_size_t x, cga_size_t y, cga_bitmap_t* bmp);

#endif
