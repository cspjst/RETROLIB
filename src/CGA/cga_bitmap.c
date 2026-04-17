/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.5
 */
#include "cga_bitmap.h"
#include "cga_bitmap_constants.h"
#include <errno.h>
#include <stdio.h>

// helper function
dos_memsize_t cga_read_rgb(FILE* f, cga_rgb_t* rgb) {
    unsigned char buff[3];
    errno = EINVAL;
    if (!f || !rgb) return 0;
    errno = EIO;
    int n = fread(buff, 1, 3, f);   // read 3 bytes into buffer
    if (n != 3) return n;
    *rgb = 0;                       // ensure bits 24-31 zero
    *rgb |= ((unsigned long)buff[0] << 16) |    // red bits 16-23
            ((unsigned long)buff[1] << 8)  |    // green bits 8-15
            buff[2];                            // blue bits 0-7
    errno = 0;                      // success
    return 3;
}

cga_bitmap_t* cga_make_bmp(cga_bitmap_t* bmp, cga_colour_depth_t depth, cga_coord_t width, cga_coord_t height, unsigned int pal) {
    if(!bmp || width == 0 || height == 0) return NULL;
    bmp->depth  = depth;
    bmp->width  = width;
    bmp->height = height;
    bmp->size = ((dos_memsize_t)width * depth + 7) >> 3 * bmp->height;
    bmp->palette = pal;
    bmp->data = NULL;
    return bmp;
}

FILE* cga_read_meta_raw_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f) return NULL;
    char line[10];
    if(!fgets(line, sizeof(line), f)) return NULL;  // read first line
    if(*(unsigned short*)line != CGA_RAW_PBM) return NULL; // only "P4" is valid
    if(!fgets(line, sizeof(line), f)) return NULL;  // read default colour
    if (sscanf(line, "CGA_FG:%d", &bmp->palette) != 1) return NULL; // extract colour number
    while (fgets(line, sizeof(line), f)) {      // read until dimensions
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, "%hu %hu", &bmp->width, &bmp->height) == 2) {
            bmp->depth = 1;                     // mode 6
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

FILE* cga_read_meta_raw_ppm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;
    char line[10];
    unsigned short max;
    if(!fgets(line, sizeof(line), f)) return NULL;  // read first line
    if(*(unsigned short*)line != CGA_RAW_PPM) return NULL; // only "P6" is valid
    if(!fgets(line, sizeof(line), f)) return NULL;  // read palette meta data
    if (sscanf(line, "CGA_PAL:%d", &bmp->palette) != 1) return NULL; // extract palette number
    while (fgets(line, sizeof(line), f)) {      // read until dimensions
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, "%hu %hu", &bmp->width, &bmp->height) == 2) {
            bmp->depth = 2;                     // mode 4/5
            errno = 0;                          // reset the POSIX error number
            break;
        }
    }
    while (fgets(line, sizeof(line), f)) {      // read until max value
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, "%hu", &max) == 1) {
            errno = 0;                          // reset the POSIX error number
            return f;                           // a valid PPM file to work with
        }
    }
    return NULL;
}
