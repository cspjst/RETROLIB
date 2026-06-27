/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 */
#include "cga_convert.h"
#include "cga_bitmap.h"
#include "cga_bitmap_constants.h"
#include "cga_types.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char cga_convert_rgb_to_bit_pair(cga_argb_t rgb) {
    switch(rgb.red) {
        case 0x00:
            switch(rgb.green) {
                case 0x00: return 0;  // #000000 black
                case 0xAA: return 1;  // #00AA00 or #00AAAA (green or cyan)
                default:   return -1;
            }
        case 0x55:
            if(rgb.green == 0xFF) return 1;  // lt green or lt cyan
            return -1;
        case 0xAA:
            switch(rgb.green) {
                case 0x00: return 2;  // red or magenta
                case 0x55: return 3;  // brown
                case 0xAA: return 3;  // lt gray
                default:   return -1;
            }
        case 0xFF:
            switch(rgb.green) {
                case 0x55: return 2;  // lt red or lt magenta
                case 0xFF: return 3;  // yellow or white
                default:   return -1;
            }
        default:
            errno = EINVAL;
            perror(__FUNCTION__);
            return -1;
    }
}

FILE* cga_convert_read_meta_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp) { perror(__FUNCTION__); return NULL; }
    char line[15];
    if(!fgets(line, sizeof(line), f)) { perror(__FUNCTION__); return NULL; }  // read first line
    errno = EINVAL;
    if(*(unsigned short*)line != CGA_RAW_PBM) { perror(__FUNCTION__); return NULL; } // only "P4" is valid
    if(!fgets(line, sizeof(line), f)) { perror(__FUNCTION__); return NULL; }  // read default colour
    while (fgets(line, sizeof(line), f)) {      // read until dimensions
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, CGA_PBM_WIDTH_HEIGHT, &bmp->width, &bmp->height) == 2) {
            bmp->depth = 1;                     // mode 6
            bmp->size = ((bmp->width * bmp->depth)  >> 3) * bmp->height;
            bmp->blocks = 0;
            errno = 0;                          // reset the POSIX error number
            return f;                           // a valid PBM file to work with
        }
    }
    perror(__FUNCTION__);
    return NULL;                                // the PBM header was malformed
}

dos_memsize_t cga_convert_read_data_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp || !bmp->data[0]) { perror(__FUNCTION__); return 0; }
    if (fread(bmp->data[0], 1, bmp->size, f) != bmp->size) { perror(__FUNCTION__); return 0; }
    bmp->blocks = 1;
    errno = 0;                                  // reset the POSIX error number
    return bmp->size;                           // success
}

cga_bitmap_t* cga_convert_load_pbm(const char* pbmfile, mem_arena_t* arena) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!pbmfile || !arena) { perror(__FUNCTION__); return NULL; } // failed: null arguments
    // allocate bitmap descriptor from arena
    cga_bitmap_t* bmp = (cga_bitmap_t*)mem_arena_alloc(arena, sizeof(cga_bitmap_t));
    if(!bmp) { perror(__FUNCTION__); return NULL; } // failed: arena OOM (errno set by arena)
    FILE* f = fopen(pbmfile, "rb");       // open source PBM file for binary read
    if(!f) { perror(__FUNCTION__); return NULL; } // failed: fopen error (errno set by fopen)
    // parse PBM header
    if(!cga_convert_read_meta_pbm(f, bmp)) { perror(__FUNCTION__); return NULL; } // failed: malformed header
    // allocate packed 1bpp pixel buffer from arena                            // POSIX error Not enough space
    bmp->data[0] = (char*)mem_arena_alloc(arena, bmp->size);
    if(!bmp->data[0]) { perror(__FUNCTION__); return NULL; }          // failed: arena OOM for pixel data
    // PBM format bitstream -> 1bpp packed scanlines
    if(!cga_convert_read_data_pbm(f, bmp)) { perror(__FUNCTION__); return NULL; } // failed: I/O or read error
    fclose(f);                                  // close source file
    errno = 0;                                  // reset POSIX error number
    return bmp;                                 // success
}

dos_memsize_t cga_convert_pbm_to_cga(const char* pbmfile, const char* cgafile, mem_arena_t* arena) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!pbmfile || !cgafile || !arena) { perror(__FUNCTION__); return 0; } // failed: null arguments
    // convert PPM file to cga_bitmap_t
    cga_bitmap_t* bmp = cga_convert_load_pbm(pbmfile, arena);
    if(!bmp) { perror(__FUNCTION__); return 0; } // failed: errno set by loader
    // save as raw cga_bitmap_t format (header + packed 2bpp payload)
    return cga_bmp_save(cgafile, bmp); // success: bytes written, or 0 on fail (errno set)
}

FILE* cga_convert_read_meta_ppm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp) { perror(__FUNCTION__); return NULL; }
    char line[15];                              // buffer for meta data
    unsigned short max;                         // PPM maximum color value
    if(!fgets(line, sizeof(line), f)) { perror(__FUNCTION__); return NULL; }  // read first line
    errno = EINVAL;
    if(*(unsigned short*)line != CGA_RAW_PPM) { perror(__FUNCTION__); return NULL; } // only "P6" is valid
    if(!fgets(line, sizeof(line), f)) { perror(__FUNCTION__); return NULL; } // read palette meta data
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
            bmp->blocks = 0;
            for(int i = 0; i < 8; ++i) bmp->data[i] = NULL;
            errno = 0;                          // reset the POSIX error number
            return f;                           // a valid PPM file to work with
        }
    }
    perror(__FUNCTION__);
    return NULL;
}

dos_memsize_t cga_convert_read_data_ppm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!f || !bmp) { perror(__FUNCTION__); return 0; }
    const cga_size_t size = bmp->width * 3;     // bytes per row: 3 × RGB per pixel
    errno = ENOMEM;                             // POSIX error Not enough space
    char* row = malloc(size);                   // temp read buffer for one RGB row
    if (!row) { perror(__FUNCTION__); return 0; } // failed

    char* byte = bmp->data[0];                  // output cursor: packed 2bpp bytes
    cga_argb_t colour = {0};                    // reusable RGB accumulator

    for (int i = 0; i < bmp->height; ++i) {     // process each scanline
        if (fread(row, 1, size, f) != size) {   // read one RGB row
            free(row);                          // failed, clean up temp buffer
            perror(__FUNCTION__);
            return 0;
        }
        char* rgb = row;                        // input cursor: unpacked RGB triples
        const char* row_end = row + size;       // sentinel for row boundary
        // process a row in 4 pixel packed 2bpp byte chunks...
        while (rgb < row_end) {
            colour.red = *rgb++; colour.green = *rgb++; colour.blue = *rgb++;
            *byte = cga_convert_rgb_to_bit_pair(colour) << 6; // pixel 0
            colour.red = *rgb++; colour.green = *rgb++; colour.blue = *rgb++;
            *byte |= cga_convert_rgb_to_bit_pair(colour) << 4; // pixel 1
            colour.red = *rgb++; colour.green = *rgb++; colour.blue = *rgb++;
            *byte |= cga_convert_rgb_to_bit_pair(colour) << 2; // pixel 2
            colour.red = *rgb++; colour.green = *rgb++; colour.blue = *rgb++;
            *byte |= cga_convert_rgb_to_bit_pair(colour);      // pixel 3
            byte++;                             // next packed destination byte
        }
    }
    bmp->blocks = 1;
    free(row);                                  // free temp row buffer
    return bmp->size;                           // success
}

cga_bitmap_t* cga_convert_load_ppm(const char* ppmfile, mem_arena_t* arena) {
    errno = EINVAL;                                 // POSIX error Invalid Argument
    if(!ppmfile || !arena) { perror(__FUNCTION__); return NULL; }
    printf("allocate bitmap descriptor from arena\n");
    cga_bitmap_t* bmp = (cga_bitmap_t*)mem_arena_alloc(arena, sizeof(cga_bitmap_t));
    if(!bmp) { perror(__FUNCTION__); return NULL; } // failed: arena OOM (errno set by arena)
    printf("open %s for binary read\n", ppmfile);
    FILE* f = fopen(ppmfile, "rb");           // open PPM file for binary read
    if(!f) { perror(ppmfile); return NULL; }  // failed: fopen error (errno set by fopen)
    printf("parse PPM header first\n");
    if(!cga_convert_read_meta_ppm(f, bmp)) { perror(__FUNCTION__); return NULL; }  // failed: malformed header
    printf("allocate packed 2bpp pixel buffer from arena\n");
    bmp->data[0] = (char*)mem_arena_alloc(arena, bmp->size);
    if(!bmp->data[0]) { perror(__FUNCTION__); return NULL; }    // failed: arena OOM for pixel data
    printf("convert RGB rows to packed 2bpp scanlines\n wait...\n");
    if(!cga_convert_read_data_ppm(f, bmp)) { perror(__FUNCTION__); return NULL; } // failed: I/O or conversion error
    fclose(f);                                  // close PPM file
    errno = 0;                                  // reset POSIX error number
    return bmp;                                 // success
}

dos_memsize_t cga_convert_ppm_to_cga(const char* ppmfile, const char* cgafile, mem_arena_t* arena) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!ppmfile || !cgafile || !arena) { perror(__FUNCTION__); return 0; }
    printf("load and convert %s PPM file.\n", ppmfile);
    cga_bitmap_t* bmp = cga_convert_load_ppm(ppmfile, arena);
    if(!bmp) { perror(__FUNCTION__); return 0; } // failed: errno set by loader
    printf("save as %s (header + packed 2bpp payload)\n", cgafile);
    return cga_bmp_save(cgafile, bmp); // success: bytes written, or 0 on fail (errno set)
}
