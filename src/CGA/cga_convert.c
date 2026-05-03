#include "cga_convert.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cga_bitmap.h"
#include "cga_bitmap_constants.h"
#include "cga_colours.h"

/*static const uint16_t CONVERT_RGB_TABLE[26] = {
    0x0000, 0x0000,
    0x0100, 0xAA00,
    0x0100, 0xAAAA,
    0x02AA, 0x0000,
    0x02AA, 0x00AA,
    0x03AA, 0x5500,
    0x03AA, 0xAAAA,
    0x0155, 0xFF55,
    0x0155, 0xFFFF,
    0x02FF, 0x5555,
    0x02FF, 0x55FF,
    0x03FF, 0xFF55,
    0x03FF, 0xFFFF,
};*/

static const uint16_t CONVERT_RGB_TABLE[26] = {
    0x0100, 0x0000,
    0x0200, 0xAA00,
    0x0300, 0xAAAA,
    0x04AA, 0x0000,
    0x05AA, 0x00AA,
    0x06AA, 0x5500,
    0x07AA, 0xAAAA,
    0x0855, 0xFF55,
    0x0955, 0xFFFF,
    0x0AFF, 0x5555,
    0x0BFF, 0x55FF,
    0x0CFF, 0xFF55,
    0x0DFF, 0xFFFF,
};

char cga_convert_rgb_to_bit_pair(uint32_t* rgb) {
    char bits;
    __asm {
        .8086
        push    es
        push    si

        les     si, rgb
        mov     ax, es:[si]          ; AX = low word RGB (bits 15-0)
        mov     dx, es:[si+2]        ; DX = high word (bits 31-16)
        and     dx, 0FFh             ; DX = high byte RGB (bits 23-16)

        mov     ah, 0FFh             ; Default not found

        ; Index 6 (midpoint)
        mov     bx, CONVERT_RGB_TABLE[24]     ; BX = low word (value in BH)
        mov     cx, CONVERT_RGB_TABLE[24+2]   ; CX = high word (key_high in CL)
        cmp     cl, dl
        jb      I9
        ja      I2
        cmp     al, bl
        jb      I9
        ja      I2
        mov     ah, bh
        jmp     DONE

I2:     ; Index 2
        mov     bx, CONVERT_RGB_TABLE[8]
        mov     cx, CONVERT_RGB_TABLE[8+2]
        cmp     cl, dl
        jb      I4
        ja      I0
        cmp     al, bl
        jb      I4
        ja      I0
        mov     ah, bh
        jmp     DONE

I0:     ; Index 0
        mov     bx, CONVERT_RGB_TABLE[0]
        mov     cx, CONVERT_RGB_TABLE[0+2]
        cmp     cl, dl
        jne     I1
        cmp     al, bl
        je      FOUND
I1:     ; Index 1
        mov     bx, CONVERT_RGB_TABLE[4]
        mov     cx, CONVERT_RGB_TABLE[4+2]
        cmp     cl, dl
        jne     DONE
        cmp     al, bl
        jne     DONE
        jmp     FOUND

I4:     ; Index 4
        mov     bx, CONVERT_RGB_TABLE[16]
        mov     cx, CONVERT_RGB_TABLE[16+2]
        cmp     cl, dl
        jb      I5
        ja      I3
        cmp     al, bl
        jb      I5
        ja      I3
        mov     ah, bh
        jmp     DONE
I3:     ; Index 3
        mov     bx, CONVERT_RGB_TABLE[12]
        mov     cx, CONVERT_RGB_TABLE[12+2]
        cmp     cl, dl
        jne     DONE
        cmp     al, bl
        jne     DONE
        jmp     FOUND
I5:     ; Index 5
        mov     bx, CONVERT_RGB_TABLE[20]
        mov     cx, CONVERT_RGB_TABLE[20+2]
        cmp     cl, dl
        jne     DONE
        cmp     al, bl
        jne     DONE

I9:     ; Index 9
        mov     bx, CONVERT_RGB_TABLE[36]
        mov     cx, CONVERT_RGB_TABLE[36+2]
        cmp     cl, dl
        jb      I11
        ja      I7
        cmp     al, bl
        jb      I11
        ja      I7
        mov     ah, bh
        jmp     DONE

I7:     ; Index 7
        mov     bx, CONVERT_RGB_TABLE[28]
        mov     cx, CONVERT_RGB_TABLE[28+2]
        cmp     cl, dl
        jne     I8
        cmp     al, bl
        je      FOUND
I8:     ; Index 8
        mov     bx, CONVERT_RGB_TABLE[32]
        mov     cx, CONVERT_RGB_TABLE[32+2]
        cmp     cl, dl
        jne     DONE
        cmp     al, bl
        jne     DONE
        jmp     FOUND

I11:    ; Index 11
        mov     bx, CONVERT_RGB_TABLE[44]
        mov     cx, CONVERT_RGB_TABLE[44+2]
        cmp     cl, dl
        jb      I12
        ja      I10
        cmp     al, bl
        jb      I12
        ja      I10
        mov     ah, bh
        jmp     DONE
I10:    ; Index 10
        mov     bx, CONVERT_RGB_TABLE[40]
        mov     cx, CONVERT_RGB_TABLE[40+2]
        cmp     cl, dl
        jne     I12
        cmp     al, bl
        je      FOUND
I12:    ; Index 12
        mov     bx, CONVERT_RGB_TABLE[48]
        mov     cx, CONVERT_RGB_TABLE[48+2]
        cmp     cl, dl
        jne     DONE
        cmp     al, bl
        jne     DONE

FOUND:  mov     ah, bh

DONE:   mov     bits, ah
        pop     si
        pop     es
    }
    return bits;
}

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
    return cga_bmp_save(pbm_file_out_path, bmp, 0); // success: bytes written, or 0 on fail (errno set)
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
            bmp->shifts = 0;
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
            *byte = cga_convert_rgb_to_pixel(palette, colour) << 6; // pixel 0
            colour.red = *rgb++; colour.green = *rgb++; colour.blue = *rgb++;
            *byte |= cga_convert_rgb_to_pixel(palette, colour) << 4; // pixel 1
            colour.red = *rgb++; colour.green = *rgb++; colour.blue = *rgb++;
            *byte |= cga_convert_rgb_to_pixel(palette, colour) << 2; // pixel 2
            colour.red = *rgb++; colour.green = *rgb++; colour.blue = *rgb++;
            *byte |= cga_convert_rgb_to_pixel(palette, colour);      // pixel 3
            byte++;                             // next packed destination byte
        }
    }
    free(row);                                  // free temp row buffer
    return bmp->size;                           // success
}

cga_bitmap_t* cga_convert_load_ppm(const char* ppm_file_path, mem_arena_t* arena) {
    errno = EINVAL;                             // POSIX error Invalid Argument
    if(!ppm_file_path || !arena) return NULL;   // failed: null arguments
    // allocate bitmap descriptor from arena
    cga_bitmap_t* bmp = (cga_bitmap_t*)mem_arena_alloc(arena, sizeof(cga_bitmap_t));
    if(!bmp) return NULL;                       // failed: arena OOM (errno set by arena)
    FILE* f = fopen(ppm_file_path, "rb");       // open PPM file for binary read
    if(!f) return NULL;                         // failed: fopen error (errno set by fopen)
    // parse PPM header first
    if(!cga_convert_read_meta_ppm(f, bmp)) return NULL; // failed: malformed header
    // allocate packed 2bpp pixel buffer from arena
    bmp->data[0] = (char*)mem_arena_alloc(arena, bmp->size);
    if(!bmp->data[0]) return NULL;              // failed: arena OOM for pixel data
    // convert RGB rows to packed 2bpp scanlines
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
    // convert PPM file to cga_bitmap_t
    cga_bitmap_t* bmp = cga_convert_load_ppm(ppm_file_in_path, arena);
    if(!bmp) return 0;                          // failed: errno set by loader
    // save as raw cga_bitmap_t format (header + packed 2bpp payload)
    return cga_bmp_save(ppm_file_out_path, bmp, 0); // success: bytes written, or 0 on fail (errno set)
}
