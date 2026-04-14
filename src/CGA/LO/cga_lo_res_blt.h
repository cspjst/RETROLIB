/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 *
 * CGA "low resolution" 320x200 pixels width:height ratio 1.6
 * Four sets of 3 sprite optimised fast blt routine:
 * Bitmaps and Sprites aesthetic ratio 1.33 width:height for CGA lo-res mode
 * 32x24 large
 * 24x18 medium
 * 16x12 small
 *  8x6  tiny
 */
#ifndef CGA_LO_RES_BLT_H
#define CGA_LO_RES_BLT_H

#include "../cga_types.h"

void cga_lo_res_screen_blt(const char* data);

char cga_lo_res_sync_blt8x6(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_lo_res_blt8x6(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_lo_res_cpy8x6(cga_coord_t x, cga_coord_t y, char* buffer);

char cga_lo_res_sync_blt16x12(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_lo_res_blt16x12(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_lo_res_cpy16x12(cga_coord_t x, cga_coord_t y, char* buffer);

char cga_lo_res_sync_blt24x18(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_lo_res_blt24x18(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_lo_res_cpy24x18(cga_coord_t x, cga_coord_t y, char* buffer);

char cga_lo_res_sync_blt32x24(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_lo_res_blt32x24(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_lo_res_cpy32x24(cga_coord_t x, cga_coord_t y, char* buffer);

// general purpose slower verisions
char cga_lo_res_sync_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* bmp);
void cga_lo_res_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* bmp);
void cga_lo_res_cpy(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, char* buffer);


#endif
