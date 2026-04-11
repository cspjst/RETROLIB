/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#ifndef CGA_HI_RES_BLT_H
#define CGA_HI_RES_BLT_H

#include "cga_types.h"

void cga_hi_res_screen_blt(const char* data);

void cga_hi_res_blt4x4(cga_coord_t x, cga_coord_t y, const char* data);

void cga_hi_res_blt8x8(cga_coord_t x, cga_coord_t y, const char* data);

void cga_hi_res_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* data);


#endif
