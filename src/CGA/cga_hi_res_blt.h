/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#ifndef CGA_HI_RES_BLT_H
#define CGA_HI_RES_BLT_H

#include "cga_types.h"

void cga_hi_res_fill_vram(const char* data);

void __fastcall cga_hi_res_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* data);

#endif
