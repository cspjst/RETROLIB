/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#ifndef CGA_LO_BLT_H
#define CGA_LO_BLT_H

#include "../cga_types.h"

void cga_lo_screen_blt(const char* data);

void cga_lo_set_blt(cga_coord_t x, cga_coord_t y, cga_size_t width, cga_size_t height, const char** ptrs);

void cga_lo_get_blt(cga_coord_t x, cga_coord_t y, cga_size_t width, cga_size_t height, const char** ptrs);

void cga_lo_and_blt(cga_coord_t x, cga_coord_t y, cga_size_t width, cga_size_t height, const char** ptrs);

void cga_lo_or_blt(cga_coord_t x, cga_coord_t y, cga_size_t width, cga_size_t height, const char** ptrs);

void cga_lo_xor_blt(cga_coord_t x, cga_coord_t y, cga_size_t width, cga_size_t height, const char** ptrs);



#endif
