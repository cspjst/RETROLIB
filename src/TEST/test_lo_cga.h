#ifndef TEST_LO_CGA_H
#define TEST_LO_CGA_H

#include <stdint.h>
#include <string.h>

#include "../CGA/cga_bitmap.h"

void make_data_2bit(cga_bitmap_t* bmp) {
    if(!bmp || !bmp->data) return;
    uint16_t bpl = bmp->width / 4;           // bytes per line (2 bits/pixel)
    unsigned char* data = (unsigned char*)bmp->data;
    for (uint16_t y = 0; y < bmp->height; y++) {
        unsigned char* row = data + (y * bpl);
        switch (y % 4) {
            case 0: memset(row, 0x00, bpl); break;  // Black
            case 1: memset(row, 0x55, bpl); break;  // Color 1
            case 2: memset(row, 0xAA, bpl); break;  // Color 2
            case 3: memset(row, 0xFF, bpl); break;  // Color 3
        }
    }
}

void test_lo_cga() {

}

#endif
