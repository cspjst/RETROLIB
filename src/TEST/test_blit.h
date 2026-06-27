#ifndef TEST_BLIT_H
#define TEST_BLIT_H

#include "../CGA/cga_bitmap.h"
#include "../CGA/LO/cga_lo_blit.h"
#include "../CGA/LO/cga_lo_scroll.h"

#include "../ENV/env_video_mode.h"

#include <assert.h>
#include <stdio.h>

void test_cga_lo_blit() {
    mem_arena_t* arena = mem_new_arena(4096);
    cga_bitmap_t* bmp = cga_bmp_load("../res/spade_a.cga", arena);
    bmp->data[1] = (char*)mem_arena_alloc(arena, bmp->size);
    cga_lo_scroll_right(bmp->data[0], bmp->data[1], bmp->width, bmp->height, 0);
    bmp->data[2] = (char*)mem_arena_alloc(arena, bmp->size);
    cga_lo_scroll_right(bmp->data[1], bmp->data[2], bmp->width, bmp->height, 0);
    bmp->data[3] = (char*)mem_arena_alloc(arena, bmp->size);
    cga_lo_scroll_right(bmp->data[2], bmp->data[3], bmp->width, bmp->height, 0);
    cga_bmp_dump(stderr, bmp);

    getchar();
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_GRAY_320X200);

    cga_lo_blit(96, 10, bmp);
    cga_lo_blit(8, 2, bmp);
    cga_lo_blit(9, 12, bmp);
    cga_lo_blit(10, 22, bmp);
    cga_lo_blit(11, 32, bmp);
    cga_lo_blit(12, 42, bmp);

    getchar();
    mem_free_arena(arena);
    env_set_video_mode(m);
}

void test_blit() {
    test_cga_lo_blit();
}

#endif
