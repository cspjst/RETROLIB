/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#ifndef CGA_HI_RES_BLT_H
#define CGA_HI_RES_BLT_H

#include "../cga_types.h"

void cga_hi_res_screen_blt(const char* data);

// returns 0 on success ie in vsync else 1 and does nothing then wait for vysnc again
char cga_hi_res_sync_blt4x4(cga_coord_t x, cga_coord_t y, const char* bmp);
// blt 4x4 bitmap
void cga_hi_res_blt4x4(cga_coord_t x, cga_coord_t y, const char* bmp);
// copies byte aligned background to buffer
void cga_hi_res_cpy4x4(cga_coord_t x, cga_coord_t y, char* buffer);

char cga_hi_res_sync_blt8x8(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_blt8x8(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_cpy8x8(cga_coord_t x, cga_coord_t y, char* buffer);

char cga_hi_res_sync_blt16x16(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_blt16x16(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_cpy16x16(cga_coord_t x, cga_coord_t y, char* buffer);

char cga_hi_res_sync_blt24x24(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_blt24x24(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_cpy24x24(cga_coord_t x, cga_coord_t y, char* buffer);

char cga_hi_res_sync_blt32x32(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_blt32x32(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_cpy32x32(cga_coord_t x, cga_coord_t y, char* buffer);

// general purpose slower verisions
char cga_hi_res_sync_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* bmp);
void cga_hi_res_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* bmp);
void cga_hi_res_cpy(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, char* buffer);


#endif
