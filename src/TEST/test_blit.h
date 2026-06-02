#ifndef TEST_BLIT_H
#define TEST_BLIT_H

#include "../CGA/cga_bitmap.h"
#include "../CGA/LO/cga_lo_blit.h"
#include "../ENV/env_video_mode.h"

#include <assert.h>

void test_cga_lo_blit() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_GRAY_320X200);
    mem_arena_t* arena = mem_new_arena(4096);
    cga_bitmap_t* bmp = cga_bmp_load("../res/spade_a.cga", arena);
    //cga_bmp_dump(stderr, bmp);

    cga_lo_paste_blit_36(96, 10, bmp);
    //cga_lo_paste_blit_36(97, 20, bmp);
    //cga_lo_paste_blit_36(98, 30, bmp);
    //cga_lo_paste_blit_36(103, 40, bmp);

    getchar();
     mem_free_arena(arena);
    env_set_video_mode(m);
}

void test_blit() {
    test_cga_lo_blit();
}

#endif
