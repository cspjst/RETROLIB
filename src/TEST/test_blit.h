#ifndef TEST_BLIT_H
#define TEST_BLIT_H

#include "../CGA/cga_bitmap.h"
#include "../CGA/LO/cga_lo_blit.h"

#include <assert.h>

void test_cga_lo_blit() {
    cga_bitmap_t bmp;
    assert(cga_make_bmp(&bmp, 2, 64, 64));
    cga_bmp_dump(stdout, &bmp);
}

void test_blit() {
    test_cga_lo_blit();
}

#endif
