/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.2.3
 *
 * Slower than the tile functions these block transfer "blt" functions
 * shift the bitmap data across byte boundaries for bitmaps that do not have
 * preshifted optimised bitmap duplication.
 *
 * CGA "hi resolution" 640x200 pixels has a very distorted width:height ratio 3.2:1
 * But keep the same four sets of 3 sprite optimised fast blt routines as lo res mode:
 * Bitmaps and Sprites aesthetic ratio 1.33 width:height same as CGA lo-res mode
 * 32x24 large
 * 24x18 medium
 * 16x12 small
 *  8x6  tiny
 *  4x4  micro
 *
 * @note the synchronised versions of the functions will *only* write to VRAM if called
 * during a vertical retrace when they return success 0 otherwise returning immediately
 * with a value of 1 - enabling the caller to wait for a vsync before trying again.
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
