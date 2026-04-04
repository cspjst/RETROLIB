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
            bmp->res = CGA_HI_RES;              // mode 6 data
            bmp->size = ((bmp->width + 7) >> 3) * bmp->height;  // round up width to bytes
            errno = 0;                          // reset the POSIX error number
            return f;                           // a valid PBM file to work with
        }
    }
    return NULL;                                // the PBM header was malformed
}

dos_memsize_t cga_load_bmp_raw_pbm(FILE* f, cga_bitmap_t* bmp) {
    errno = EINVAL;                             // POSIX error Invalid Arguement
    if(!f) return 0;
    errno = EIO;                                // POSIX error Input Ouput
    dos_address_t even, odd, data;
    data.ptr = even.ptr = odd.ptr = (void*)bmp->data;
    odd.memloc += (bmp->size / 2);
    unsigned short size  = (bmp->width + 7) >> 3;
    for(int i = 0; i < bmp->height / 2; ++i) {
        if (fread(even.ptr, 1, size, f) != size) return 0;
        even.memloc += size;
        if (fread(odd.ptr, 1, size, f) != size) return 0;
        odd.memloc += size;
    }
    errno = 0;                                  // reset the POSIX error number
    return odd.memloc - data.memloc;
}
