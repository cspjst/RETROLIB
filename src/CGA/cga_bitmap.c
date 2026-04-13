/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.5
 */
#include "cga_bitmap.h"
#include "cga_bitmap_constants.h"
#include <errno.h>
#include <stdio.h>

FILE* cga_read_meta_raw_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f) return NULL;
    char line[10];
    if(!fgets(line, sizeof(line), f)) return NULL;  // read first line
    if(*(unsigned short*)line != CGA_RAW_PBM) return NULL; // only "P4" is valid
    while (fgets(line, sizeof(line), f)) {      // read until dimensions
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, "%hu %hu", &bmp->width, &bmp->height) == 2) {
            cga_make_bmp(bmp, 1, bmp->width, bmp->height);
            errno = 0;                          // reset the POSIX error number
            return f;                           // a valid PBM file to work with
        }
    }
    return NULL;                                // the PBM header was malformed
}

dos_memsize_t cga_load_bmp_raw_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;
    if(!f || !bmp || !bmp->data) return 0;
    errno = EIO;
    if (fread(bmp->data, 1, bmp->size, f) != bmp->size) return 0;
    errno = 0;
    return bmp->size;
}

cga_bitmap_t* cga_make_bmp(cga_bitmap_t* bmp, unsigned short depth, cga_coord_t width, cga_coord_t height) {
    if(!bmp || width == 0 || height == 0) return NULL;

    bmp->depth  = depth;
    bmp->width  = width;
    bmp->height = height;

    // bytes per line = ceil(width * depth / 8)
    int bpl = ((dos_memsize_t)width * depth + 7) >> 3;
    bmp->size = bpl * bmp->height;

    bmp->data = NULL;  // caller allocates via arena or other means
    return bmp;
}
