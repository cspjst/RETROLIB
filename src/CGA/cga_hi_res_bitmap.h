/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#ifndef CGA_HI_RES_BITMAP_H
#define CGA_HI_RES_BITMAP_H

#include "cga_types.h"
#include "../../doslib/src/DOS/dos_memory_types.h"

typedef struct {
    cga_coord_t width;
    cga_coord_t height;
    char* data;         // limited to 64K max
} cga_hi_res_bitmap_t;

dos_memsize_t cga_hi_res_load_pbm(const char* file_path);

/**
 * separate data into 2 blocks of odd and even rows
 */
char* cga_hi_res_convert_bmp(cga_hi_res_bitmap_t);

dos_memsize_t cga_hi_res_save_raw(const char* file_path);

#endif
