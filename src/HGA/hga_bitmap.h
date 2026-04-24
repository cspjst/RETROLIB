#ifndef HGA_BITMAP_T
#define HGA_BITMAP_T

#include <stdio.h>

#include "../../doslib/src/DOS/dos_memory_types.h"

#include "hga_types.h"

typedef struct {
    hga_coord_t width;          // 0..719
    hga_coord_t height;         // 0..347
    hga_size_t data_size;       // limited to 64K max
    char* data;
} hga_bitmap_t;

hga_bitmap_t* hga_make_bmp(hga_bitmap_t* bmp, hga_coord_t width, hga_coord_t height);

/**
 * The portable bitmap format (PBM) as the raw byte format is ideal for Hercules monochrome
 */
FILE* hga_bmp_read_meta_raw_pbm(FILE* f, hga_bitmap_t* bmp);
dos_memsize_t hga_bmp_load_raw_pbm(FILE* f, hga_bitmap_t* bmp);

/**
* Dump the meta data and first 16 bytes of raw data
*/
void cga_bmp_dump(FILE* f, hga_bitmap_t* bmp);

#endif
