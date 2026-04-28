/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.2.3
 */
#ifndef CGA_HI_RES_BLT_H
#define CGA_HI_RES_BLT_H

#include "../cga_types.h"

void cga_hi_res_screen_blt(const char* data);

// general purpose slower verisions
//char cga_hi_res_sync_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* bmp);
void cga_hi_res_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* bmp);
//void cga_hi_res_cpy(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, char* buffer);

// returns 0 on success ie in vsync else 1 and does nothing then wait for vsync again
//char cga_hi_res_sync_blt4x4(cga_coord_t x, cga_coord_t y, const char* bmp);
// blt 4x4 bitmap
void cga_hi_res_blt4x4(cga_coord_t x, cga_coord_t y, const char* bmp);

//char cga_hi_res_sync_blt8x8(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_blt8x6(cga_coord_t x, cga_coord_t y, const char* bmp);


#endif
