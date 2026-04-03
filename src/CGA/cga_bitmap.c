#include "cga_bitmap.h"
#include "cga_bitmap_constants.h"
#include <errno.h>

/*
FILE* fp = fopen("image.ppm", "rb");
fscanf(fp, "P6\n%d %d\n255\n", &width, &height);  // skip header
fgetc(fp); // consume newline

uint8_t* rgb_data = malloc(width * height * 3);
fread(rgb_data, 1, width * height * 3, fp);  // read all RGB triplets
fclose(fp);
 */

FILE* cga_read_meta_raw_pbm(const char* file_path, cga_bitmap_t* bmp) {
    errno = 0;                                  // reset the POSIX error number
    char line[80];
    FILE* f = fopen(file_path, "rb");
    if(!f) return f;
    fgets(line, sizeof(line), f);               // read first line
    if(*(cga_netpbm_t*)line != CGA_RAW_PBM) {   // only "P4" is valid
        errno = EINVAL;                         // POSIX error Invalid argument
        return NULL;
    }
    while (fgets(line, sizeof(line), f)) {      // read until dimensions
        if (line[0] == '#') continue;           // skip comments
        if (sscanf(line, "%hu %hu", &bmp->width, &bmp->height) == 2) {
            bmp->res = CGA_HI_RES;              // mode 6 data
            return f;                           // a valid PBM file to work with
        }
    }
    errno = EINVAL;                             // POSIX error Invalid argument
    return NULL;                                // the PBM header was malformed
}
