#ifndef CGA_LO_BLIT_H
#define CGA_LO_BLIT_H

#include "../cga_types.h"
#include "../cga_bitmap.h"

void cga_lo_blit(cga_coord_t x, cga_coord_t y, cga_bitmap_t* bmp);

void cga_lo_mask_blit(cga_coord_t x, cga_coord_t y, cga_bitmap_t* mask, cga_bitmap_t* bmp);

void cga_lo_copy_blit(cga_coord_t x, cga_coord_t y, cga_size_t w, cga_size_t h, char* data);

void cga_lo_paste_blit(cga_coord_t x, cga_coord_t y, cga_size_t w, cga_size_t h, char* data);

#endif
