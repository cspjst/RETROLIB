#include "cga_convert.h"

#include <errno.h>
#include <stdlib.h>

#include "cga_bitmap_constants.h"
#include "cga_colours.h"

char cga_bmp_rgb_to_pixel(cga_size_t palette, cga_argb_t colour) {
    errno = 0;
    if (colour.argb == 0) return 0;

    switch (palette) {
        case CGA_PALETTE_0:
            switch (colour.argb) {
                case RGB_GREEN:     return 1;
                case RGB_RED:       return 2;
                case RGB_BROWN:     return 3;
                default:
                    errno = EINVAL;
                    return 0;
            }

        case CGA_PALETTE_0_HI:
            switch (colour.argb) {
                case RGB_LT_GREEN:  return 1;
                case RGB_LT_RED:    return 2;
                case RGB_YELLOW:    return 3;
                default:
                    errno = EINVAL;
                    return 0;
            }

        case CGA_PALETTE_1:
            switch (colour.argb) {
                case RGB_CYAN:      return 1;
                case RGB_MAGENTA:   return 2;
                case RGB_LT_GRAY:   return 3;
                default:
                    errno = EINVAL;
                    return 0;
            }

        case CGA_PALETTE_1_HI:
            switch (colour.argb) {
                case RGB_LT_CYAN:   return 1;
                case RGB_LT_MAGENTA: return 2;
                case RGB_WHITE:     return 3;
                default:
                    errno = EINVAL;
                    return 0;
            }

        case CGA_PALETTE_2:
            switch (colour.argb) {
                case RGB_CYAN:      return 1;
                case RGB_RED:       return 2;
                case RGB_LT_GRAY:   return 3;
                default:
                    errno = EINVAL;
                    return 0;
            }

        case CGA_PALETTE_2_HI:
            switch (colour.argb) {
                case RGB_LT_CYAN:   return 1;
                case RGB_LT_RED:    return 2;
                case RGB_WHITE:     return 3;
                default:
                    errno = EINVAL;
                    return 0;
            }

        default:
            errno = EINVAL;
            return 0;
    }
}

FILE* cga_bmp_read_meta_raw_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp) return NULL;
    char line[10];
    if(!fgets(line, sizeof(line), f)) return NULL;  // read first line
    if(*(unsigned short*)line != CGA_RAW_PBM) return NULL; // only "P4" is valid
    if(!fgets(line, sizeof(line), f)) return NULL;  // read default colour
    if (sscanf(line, CGA_META_FG, &bmp->palette) != 1) return NULL; // colour
    while (fgets(line, sizeof(line), f)) {      // read until dimensions
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, CGA_META_WIDTH_HEIGHT, &bmp->width, &bmp->height) == 2) {
            bmp->depth = 1;                     // mode 6
            bmp->size = ((bmp->width * bmp->depth)  >> 3) * bmp->height;
            errno = 0;                          // reset the POSIX error number
            return f;                           // a valid PBM file to work with
        }
    }
    return NULL;                                // the PBM header was malformed
}

dos_memsize_t cga_bmp_load_raw_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp || !bmp->data) return 0;
    errno = EIO;                                // POSIX error Input/Output
    if (fread(bmp->data, 1, bmp->size, f) != bmp->size) return 0;
    errno = 0;                                  // reset the POSIX error number
    return bmp->size;
}

FILE* cga_bmp_read_meta_raw_ppm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp) return NULL;
    char line[15];                              // buffer for meta data
    unsigned short max;                         // PPM maximum color value
    if(!fgets(line, sizeof(line), f)) return NULL;  // read first line
    if(*(unsigned short*)line != CGA_RAW_PPM) return NULL; // only "P6" is valid
    if(!fgets(line, sizeof(line), f)) return NULL;  // read palette meta data
    if (sscanf(line, CGA_META_PAL, &bmp->palette) != 1) return NULL; // extract palette number
    while (fgets(line, sizeof(line), f)) {      // read until dimensions
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, CGA_META_WIDTH_HEIGHT, &bmp->width, &bmp->height) == 2) {
            bmp->depth = 2;                     // mode 4/5
            bmp->size = ((bmp->width * bmp->depth)  >> 3) * bmp->height;
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

dos_memsize_t cga_bmp_load_raw_ppm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;
    if(!f || !bmp || !bmp->data) return 0;

    const cga_size_t size = bmp->width * 3;
    const unsigned char palette = bmp->palette;

    errno = ENOMEM;
    char* row = malloc(size);
    if (!row) return 0;

    char* byte = bmp->data;
    cga_argb_t colour = {0};

    for (int i = 0; i < bmp->height; ++i) {
        errno = EIO;
        if (fread(row, 1, size, f) != size) {
            free(row);
            return 0;
        }
        errno = 0;

        char* rgb = row;
        const char* row_end = row + size;

        while (rgb < row_end) {
            colour.red = *rgb++;
            colour.green = *rgb++;
            colour.blue = *rgb++;
            *byte = cga_bmp_rgb_to_pixel(palette, colour) << 6;

            colour.red = *rgb++;
            colour.green = *rgb++;
            colour.blue = *rgb++;
            *byte |= cga_bmp_rgb_to_pixel(palette, colour) << 4;

            colour.red = *rgb++;
            colour.green = *rgb++;
            colour.blue = *rgb++;
            *byte |= cga_bmp_rgb_to_pixel(palette, colour) << 2;

            colour.red = *rgb++;
            colour.green = *rgb++;
            colour.blue = *rgb++;
            *byte |= cga_bmp_rgb_to_pixel(palette, colour);

            byte++;
        }
    }

    free(row);

    if (byte - bmp->data != bmp->size) return 0;

    errno = 0;
    return bmp->size;
}
