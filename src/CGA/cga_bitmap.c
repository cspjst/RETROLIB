/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.5
 */
#include "cga_bitmap.h"
#include "cga_bitmap_constants.h"
#include "cga_colours.h"
#include "cga_types.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// helper function
char cga_bmp_rgb_to_pixel(cga_size_t palette, cga_argb_t colour) {
    errno = 0;
    if(colour.argb == 0) return 0;
    switch(palette) {
        case CGA_PALETTE_1_HI:
            switch(colour.argb) {
                case RGB_LT_CYAN: return 1;
                case RGB_LT_MAGENTA: return 2;
                case RGB_WHITE: return 3;
                default:
                    errno = EINVAL;
                    return 0;
            }
        default:
            errno = EINVAL;
            return 0;
    }
}

cga_bitmap_t* cga_make_bmp(cga_bitmap_t* bmp, cga_colour_depth_t depth, cga_coord_t width, cga_coord_t height, unsigned int pal) {
    if(!bmp || width == 0 || height == 0) return NULL;
    bmp->depth  = depth;
    bmp->width  = width;
    bmp->height = height;
    bmp->size = ((bmp->width * bmp->depth + 7) >> 3) * bmp->height;
    bmp->palette = pal;
    bmp->data = NULL;
    return bmp;
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

// --------------------------------------- //
dos_memsize_t cga_bmp_load_raw_ppm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp || !bmp->data) return 0;
    cga_size_t size = bmp->width * 3;           // PPM row size
    errno = ENOMEM;                             // POSIX erroe No Memory
    char* row = malloc(size);                   // PPM row data
    if (!row) return 0;
    errno = EIO;                                // POSIX error Input/Output
    if (fread(row, 1, size, f) != size) {       // Read a row of RGB values
        free(row);                              // Read error
        return 0;
    }
    char* ppm = row;
    char* byte = bmp->data;
    cga_argb_t colour = {0};
    errno = 0;
    while(ppm < row + size) {
        // convert 4 pixels into a bitmap data byte
        colour.red = *ppm++;
        colour.green = *ppm++;
        colour.blue = *ppm++;
        *byte = cga_bmp_rgb_to_pixel(bmp->palette, colour) << 6;
        if(errno) return 0;
        colour.red = *ppm++;
        colour.green = *ppm++;
        colour.blue = *ppm++;
        *byte |= cga_bmp_rgb_to_pixel(bmp->palette, colour) << 4;
        if(errno) return 0;
        colour.red = *ppm++;
        colour.green = *ppm++;
        colour.blue = *ppm++;
        *byte |= cga_bmp_rgb_to_pixel(bmp->palette, colour) << 2;
        if(errno) return 0;
        colour.red = *ppm++;
        colour.green = *ppm++;
        colour.blue = *ppm++;
        *byte |= cga_bmp_rgb_to_pixel(bmp->palette, colour);
        if(errno) return 0;
        byte++;
    }
    free(row);                                  // Free row data
    printf("bytes read = %i\n", byte - bmp->data);
    if(byte - bmp->data != bmp->size) return 0;
    errno = 0;                                  // reset the POSIX error number
    return bmp->size;
}

void cga_bmp_dump(FILE* f, cga_bitmap_t* bmp) {
    if (!f || !bmp) return;

    fprintf(f, "CGA Bitmap:\n");
    fprintf(f, "  depth   = %hu\n", bmp->depth);
    fprintf(f, "  width   = %hu\n", bmp->width);
    fprintf(f, "  height  = %hu\n", bmp->height);
    fprintf(f, "  size    = %u\n", bmp->size);
    fprintf(f, "  palette = 0x%04X (%hu)\n", bmp->palette, bmp->palette);
    fprintf(f, "  data    = %p\n", (void*)bmp->data);

    if(bmp->data) {
        for(int i = 0; i < 16; ++i)
            fprintf(f, "%02X ", (unsigned char)bmp->data[i]);
        fprintf(f, "\n");
    }
}
