#ifndef CGA_LO_BLIT_H
#define CGA_LO_BLIT_H

#include "../cga_types.h"
#include "../cga_bitmap.h"

void cga_lo_paste_blit_36(cga_size_t x, cga_size_t y, cga_bitmap_t* bmp);

void cga_lo_and_or_blit(cga_size_t x, cga_size_t y, cga_bitmap_t* mask, cga_bitmap_t* bmp);

void cga_copy_blit(cga_size_t x, cga_size_t y, cga_bitmap_t* bmp);

#endif
