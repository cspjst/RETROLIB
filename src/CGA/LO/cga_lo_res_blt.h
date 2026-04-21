/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 *
 * Slower than the tile functions these block transfer "blt" functions
 * shift the bitmap data across byte boundaries for bitmaps that do not have
 * preshifted optimised bitmap duplication.
 *
 * CGA "low resolution" 320x200 somewhat distorted pixels width:height ratio 1.6:1
 * Four sets of 3 sprite optimised fast blt routine:
 * Bitmaps and Sprites aesthetic ratio 1.33 width:height for CGA lo-res mode
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
#ifndef CGA_LO_RES_BLT_H
#define CGA_LO_RES_BLT_H

#include "../cga_types.h"

void cga_lo_res_screen_blt(const char* data);

//char cga_lo_res_sync_blt4x4(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_lo_res_blt4x4(cga_coord_t x, cga_coord_t y, const char* bmp);

//char cga_lo_res_sync_blt8x6(cga_coord_t x, cga_coord_t y, const char* bmp);
//void cga_lo_res_blt8x6(cga_coord_t x, cga_coord_t y, const char* bmp);


#endif
