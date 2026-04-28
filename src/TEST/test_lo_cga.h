#ifndef TEST_LO_CGA_H
#define TEST_LO_CGA_H

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "../CGA/LO/cga_lo_blt.h"

#include "../CGA/cga_bitmap.h"

#include "../ENV/env_video_mode.h"

#include "../MEM/dos_mem_arena.h"

void test_lo_screen_blt() {
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");

    cga_bitmap_t* bmp = cga_bmp_load("../res/tree.cga", arena);
    if(!bmp) printf("error %s\n", strerror(errno));
    else cga_lo_screen_blt(bmp->data[0]);

    mem_free_arena(arena);
}

void test_lo_cga() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);

    test_lo_screen_blt();

    getchar();
    env_set_video_mode(m);
}

#endif
