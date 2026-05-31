#ifndef TEST_BLIT_H
#define TEST_BLIT_H

#include "../CGA/cga_bitmap.h"
#include "../CGA/LO/cga_lo_blit.h"
#include "../ENV/env_video_mode.h"

#include <assert.h>

void test_cga_lo_blit() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_GRAY_320X200);
    cga_bitmap_t bmp;
    assert(cga_make_bmp(&bmp, 2, 64, 64));
    bmp.data[0] = (char*)0xAABBCCDD;

    cga_lo_paste_blit(10, 10, &bmp);

    getchar();
    env_set_video_mode(m);
}

void test_blit() {
    test_cga_lo_blit();
}

#endif
