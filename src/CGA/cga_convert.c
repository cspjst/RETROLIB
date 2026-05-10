#include "cga_convert.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cga_bitmap.h"
#include "cga_bitmap_constants.h"
#include "cga_types.h"
#include "LO/cga_lo_scroll.h"

dos_memsize_t cga_convert_bmp_shifts_lo_res(cga_bitmap_t* bmp, mem_arena_t* arena) {
    errno = EINVAL;
    if(!bmp || !bmp->data[0] || !arena) return 0;           // must be a bmp and bmp data to shift and an arena to put them in

    return 0;
}

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
        default: return -1;
    }
}

FILE* cga_convert_read_meta_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp) return NULL;
    char line[15];
    if(!fgets(line, sizeof(line), f)) return NULL;  // read first line
    if(*(unsigned short*)line != CGA_RAW_PBM) return NULL; // only "P4" is valid
    if(!fgets(line, sizeof(line), f)) return NULL;  // read default colour
    if (sscanf(line, CGA_PBM_FG, &bmp->palette) != 1) return NULL; // colour
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
    return NULL;                                // the PBM header was malformed
}

dos_memsize_t cga_convert_read_data_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f || !bmp || !bmp->data[0]) return 0;
    if (fread(bmp->data[0], 1, bmp->size, f) != bmp->size) return 0;
    bmp->blocks = 1;
    errno = 0;                                  // reset the POSIX error number
    return bmp->size;                           // success
}

cga_bitmap_t* cga_convert_load_pbm(const char* pbm_file_path, mem_arena_t* arena) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!pbm_file_path || !arena) return NULL;   // failed: null arguments
    // allocate bitmap descriptor from arena
    cga_bitmap_t* bmp = (cga_bitmap_t*)mem_arena_alloc(arena, sizeof(cga_bitmap_t));
    if(!bmp) return NULL;                       // failed: arena OOM (errno set by arena)
    FILE* f = fopen(pbm_file_path, "rb");       // open source PBM file for binary read
    if(!f) return NULL;                         // failed: fopen error (errno set by fopen)
    // parse PBM header
    if(!cga_convert_read_meta_pbm(f, bmp)) return NULL; // failed: malformed header
    // allocate packed 1bpp pixel buffer from arena                            // POSIX error Not enough space
    bmp->data[0] = (char*)mem_arena_alloc(arena, bmp->size);
    if(!bmp->data[0]) return NULL;              // failed: arena OOM for pixel data
    // PBM format bitstream -> 1bpp packed scanlines
    if(!cga_convert_read_data_pbm(f, bmp)) return NULL; // failed: I/O or read error
    fclose(f);                                  // close source file
    errno = 0;                                  // reset POSIX error number
    return bmp;                                 // success
}

dos_memsize_t cga_convert_pbm_to_raw(
    const char* pbm_file_in_path,
    const char* pbm_file_out_path,
    mem_arena_t* arena
) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!pbm_file_in_path || !pbm_file_out_path || !arena) return 0; // failed: null arguments
    // convert PPM file to cga_bitmap_t
    cga_bitmap_t* bmp = cga_convert_load_pbm(pbm_file_in_path, arena);
    if(!bmp) return 0;                          // failed: errno set by loader
    // save as raw cga_bitmap_t format (header + packed 2bpp payload)
    return cga_bmp_save(pbm_file_out_path, bmp); // success: bytes written, or 0 on fail (errno set)
}

FILE* cga_convert_read_meta_ppm(FILE* f, cga_bitmap_t* bmp) {
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
            bmp->blocks = 0;
            for(int i = 0; i < 8; ++i) bmp->data[i] = NULL;
            errno = 0;                          // reset the POSIX error number
            return f;                           // a valid PPM file to work with
        }
    }
    return NULL;
}

dos_memsize_t cga_convert_read_data_ppm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!f || !bmp) return 0;
    const cga_size_t size = bmp->width * 3;     // bytes per row: 3 × RGB per pixel
    const unsigned char palette = bmp->palette; // CGA palette selector for conversion
    errno = ENOMEM;                             // POSIX error Not enough space
    char* row = malloc(size);                   // temp read buffer for one RGB row
    if (!row) return 0;                         // failed

    char* byte = bmp->data[0];                  // output cursor: packed 2bpp bytes
    cga_argb_t colour = {0};                    // reusable RGB accumulator

    for (int i = 0; i < bmp->height; ++i) {     // process each scanline
        errno = EIO;                            // POSIX error I/O failure
        if (fread(row, 1, size, f) != size) {   // read one RGB row
            free(row);                          // failed, clean up temp buffer
            return 0;
        }
        errno = 0;                              // reset POSIX error number
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

cga_bitmap_t* cga_convert_load_ppm(const char* ppm_file_path, mem_arena_t* arena) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!ppm_file_path || !arena) return NULL;   // failed: null arguments
    printf("allocate bitmap descriptor from arena\n");
    cga_bitmap_t* bmp = (cga_bitmap_t*)mem_arena_alloc(arena, sizeof(cga_bitmap_t));
    if(!bmp) return NULL;                       // failed: arena OOM (errno set by arena)
    FILE* f = fopen(ppm_file_path, "rb");       // open PPM file for binary read
    if(!f) return NULL;                         // failed: fopen error (errno set by fopen)
    printf("parse PPM header first\n");

    if(!cga_convert_read_meta_ppm(f, bmp)) return NULL; // failed: malformed header
    printf("allocate packed 2bpp pixel buffer from arena\n");
    bmp->data[0] = (char*)mem_arena_alloc(arena, bmp->size);
    if(!bmp->data[0]) return NULL;              // failed: arena OOM for pixel data
    printf("convert RGB rows to packed 2bpp scanlines\n wait...\n");
    if(!cga_convert_read_data_ppm(f, bmp)) return NULL; // failed: I/O or conversion error
    fclose(f);                                  // close PPM file
    errno = 0;                                  // reset POSIX error number
    return bmp;                                 // success
}

dos_memsize_t cga_convert_ppm_to_raw(
    const char* ppm_file_in_path,
    const char* ppm_file_out_path,
    mem_arena_t* arena
) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!ppm_file_in_path || !ppm_file_out_path || !arena) return 0; // failed: null arguments
    printf("convert PPM file to cga_bitmap_t\n");
    cga_bitmap_t* bmp = cga_convert_load_ppm(ppm_file_in_path, arena);
    if(!bmp) return 0;                          // failed: errno set by loader
    printf("save as raw cga_bitmap_t format (header + packed 2bpp payload)");
    return cga_bmp_save(ppm_file_out_path, bmp); // success: bytes written, or 0 on fail (errno set)
}

cga_size_t cga_convert_ppm_pal(
    const char* ppm_file_in_path,
    const char* ppm_file_out_path,
    cga_size_t pal
) {
    errno = EINVAL;                                 // POSIX error Invalid Argument
    if(!ppm_file_in_path) return 0;
    char line[80];
    printf("open file %s for read\n", ppm_file_in_path);
    FILE* fin = fopen(ppm_file_in_path, "r");       // error if file not exist
    if(!fin) return 0;
    if(!fgets(line, sizeof(line), fin)) return 0;   // read first line
    printf("confirm PPM type\n");
    if(*(unsigned short*)line != CGA_RAW_PPM) goto error; // confirm P6
    printf("open file %s for write\n", ppm_file_out_path);
    FILE* fout = fopen(ppm_file_in_path, "w");      // create if file not exist
    if(!fout) goto error;
    if(!fputs(line, fout)) goto error;
    printf("inject #CGA PAL=%i\n", pal);


error:
    fclose(fin);
    fclose(fout);
    return 0;
}
