#include "cga_convert.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "cga_bitmap.h"
#include "cga_bitmap_constants.h"
#include "cga_colours.h"

char cga_convert_rgb_to_pixel(cga_size_t palette, cga_argb_t colour) {
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

FILE* cga_convert_read_meta_raw_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp) return NULL;
    char line[10];
    if(!fgets(line, sizeof(line), f)) return NULL;  // read first line
    if(*(unsigned short*)line != CGA_RAW_PBM) return NULL; // only "P4" is valid
    if(!fgets(line, sizeof(line), f)) return NULL;  // read default colour
    if (sscanf(line, CGA_PBM_FG, &bmp->palette) != 1) return NULL; // colour
    while (fgets(line, sizeof(line), f)) {      // read until dimensions
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, CGA_PBM_WIDTH_HEIGHT, &bmp->width, &bmp->height) == 2) {
            bmp->depth = 1;                     // mode 6
            bmp->size = ((bmp->width * bmp->depth)  >> 3) * bmp->height;
            errno = 0;                          // reset the POSIX error number
            return f;                           // a valid PBM file to work with
        }
    }
    return NULL;                                // the PBM header was malformed
}

dos_memsize_t cga_convert_load_raw_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp || !bmp->data[0]) return 0;
    errno = EIO;                                // POSIX error Input/Output
    if (fread(bmp->data, 1, bmp->size, f) != bmp->size) return 0;
    errno = 0;                                  // reset the POSIX error number
    return bmp->size;
}

FILE* cga_convert_read_meta_raw_ppm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp) return NULL;
    char line[15];                              // buffer for meta data
    unsigned short max;                         // PPM maximum color value
    if(!fgets(line, sizeof(line), f)) return NULL;  // read first line
    if(*(unsigned short*)line != CGA_RAW_PPM) return NULL; // only "P6" is valid
    if(!fgets(line, sizeof(line), f)) return NULL;  // read palette meta data
    if (sscanf(line, CGA_PPM_PAL, &bmp->palette) != 1) return NULL; // extract palette number
    while (fgets(line, sizeof(line), f)) {      // read until dimensions
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, CGA_PBM_WIDTH_HEIGHT, &bmp->width, &bmp->height) == 2) {
            bmp->depth = 2;                     // mode 4/5
            bmp->size = ((bmp->width * bmp->depth)  >> 3) * bmp->height;
            errno = 0;                          // reset the POSIX error number
            break;
        }
    }
    while (fgets(line, sizeof(line), f)) {      // read until max value
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, CGA_PPM_MAXVAL, &max) == 1) {
            if(max > CGA_MAX_COLOUR) return NULL;
            bmp->shifts = 0;
            for(int i = 0; i < 8; ++i) bmp->data[i] = NULL;
            errno = 0;                          // reset the POSIX error number
            return f;                           // a valid PPM file to work with
        }
    }
    return NULL;
}

dos_memsize_t cga_convert_load_raw_ppm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;
    if(!f || !bmp) return 0;
    const cga_size_t size = bmp->width * 3;
    const unsigned char palette = bmp->palette;

    errno = ENOMEM;
    char* row = malloc(size);
    if (!row) return 0;

    char* byte = bmp->data[0];
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
            *byte = cga_convert_rgb_to_pixel(palette, colour) << 6;

            colour.red = *rgb++;
            colour.green = *rgb++;
            colour.blue = *rgb++;
            *byte |= cga_convert_rgb_to_pixel(palette, colour) << 4;

            colour.red = *rgb++;
            colour.green = *rgb++;
            colour.blue = *rgb++;
            *byte |= cga_convert_rgb_to_pixel(palette, colour) << 2;

            colour.red = *rgb++;
            colour.green = *rgb++;
            colour.blue = *rgb++;
            *byte |= cga_convert_rgb_to_pixel(palette, colour);

            byte++;
        }
    }

    free(row);

    if (byte - bmp->data[0] != bmp->size) return 0;

    errno = 0;
    return bmp->size;
}

cga_bitmap_t* cga_convert_load_ppm(const char* file_path, mem_arena_t* arena) {
    errno = EINVAL;
    if(!file_path || !arena) return NULL;
    cga_bitmap_t* bmp = (cga_bitmap_t*)mem_arena_alloc(arena, sizeof(cga_bitmap_t));
    if(!bmp) return NULL;            // errno will be set to ENOMEM
    FILE* f = fopen(file_path, "rb");
    if(!f) return NULL;
    if(!cga_convert_read_meta_raw_ppm(f, bmp)) return NULL;
    bmp->data[0] = (char*)mem_arena_alloc(arena, bmp->size);
    if(!cga_convert_load_raw_ppm(f, bmp)) return NULL;
    fclose(f);
    errno = 0;
    return bmp;
}

dos_memsize_t cga_convert_ppm_to_raw(
    const char* file_in_path,
    const char* file_out_path,
    mem_arena_t* arena
) {
    errno = EINVAL;
    if(!file_in_path || !file_out_path || !arena) return 0;
    cga_bitmap_t* bmp = cga_convert_load_ppm(file_in_path, arena);
    if(!bmp) return 0;       // errno already set by loader
    return cga_bmp_save(file_out_path, bmp);    // save as raw cga_bitmap_t format
}
