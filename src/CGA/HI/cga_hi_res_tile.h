/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.2.3
 *
 * Fast tiling functions these block transfer functions are byte aligned
 * ignoring the lower 7 bits of the x position being designed for use with
 * preshifted optimised bitmap duplications.
 *
 * Fast copy functions read out the VRAM tiles to memory enabling animation.
 *
 * CGA "hi resolution" 640x200 pixels has a very distorted width:height ratio 3.2:1
 * But keep the same four sets of 3 sprite optimised fast tile routines as lo res mode:
 * Bitmaps and Sprites aesthetic ratio 1.33 width:height same as CGA lo-res mode
 * 32x24 large
 * 24x18 medium
 * 16x12 small
 *  8x6  tiny
 *  4x4  micro
 *
 * Optimised square tiling functions:
 * 8x8
 * 16x16
 * 24x24
 * 32x3
 *
 * @note the synchronised versions of the functions will *only* write to VRAM if called
 * during a vertical retrace when they return success 0 otherwise returning immediately
 * with a value of 1 - enabling the caller to wait for a vsync before trying again.
 */
#ifndef CGA_HI_RES_TILE_H
#define CGA_HI_RES_TILE_H

#include "../cga_types.h"

//char cga_hi_res_sync_blt_tile(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* bmp);
//void cga_hi_res_blt_tile(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* bmp);
//void cga_hi_res_cpy_tile(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* bmp);

//char cga_hi_res_sync_blt_tile_8x6(cga_coord_t x, cga_coord_t y, const char* bmp);
void cga_hi_res_blt_tile_8x6(cga_coord_t x, cga_coord_t y, const char* bmp);
//void cga_hi_res_cpy_tile_8x6(cga_coord_t x, cga_coord_t y, const char* bmp);


#endif
