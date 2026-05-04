#ifndef TEST_LO_CGA_H
#define TEST_LO_CGA_H

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "../CGA/LO/cga_lo_blt.h"
#include "../CGA/LO/cga_lo_plot.h"
#include "../CGA/cga_bitmap.h"
#include "../CGA/cga_colours.h"

#include "../ENV/env_video_mode.h"
#include "../ENV/env_time.h"

#include "../MEM/dos_mem_arena.h"

#include "../../doslib/bioslib/src/BIOS/bios_clock_services.h"

void test_lo_screen_blt() {
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");

    cga_bitmap_t* bmp = cga_bmp_load("../res/tree1.cga", arena, 1);
    if(!bmp) printf("error %s\n", strerror(errno));
    else cga_lo_screen_blt(bmp->data[0]);

    //cga_bmp_dump(stdout, bmp);

    mem_free_arena(arena);
}

void test_lo_plot() {
    bios_ticks_since_midnight_t t1, t2;

    bios_read_system_clock(&t1);
    for(int y = 0; y < 200; ++y) {
        for(int x = 0; x < 320; ++x) {
            cga_lo_plot(x, y, CGA_LO_RES_CYAN);
            cga_lo_plot(x, y, CGA_LO_RES_BLACK);
            cga_lo_plot(x, y, CGA_LO_RES_WHITE);
            cga_lo_plot(x, y, CGA_LO_RES_MAGENTA);
        }
    }
    bios_read_system_clock(&t2);
    printf("Time = %fsec\n", env_ticks_to_seconds(t2-t1));
}

void test_lo_blt() {
    const char* ptrs[4] = {(char*)0xAAAAFF01, (char*)0xBBBBFF02, (char*)0xCCCCFF03, (char*)0xDDDDFF04};
    cga_lo_set_blt(0, 0, 1, 1, &ptrs[0]);
    cga_lo_set_blt(1, 0, 1, 1, &ptrs[0]);
    cga_lo_set_blt(6, 0, 1, 1, &ptrs[0]);
    cga_lo_set_blt(7, 0, 1, 1, &ptrs[0]);
}

void test_lo_cga() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);

    test_lo_screen_blt();
    //test_lo_plot();
    //test_lo_blt();

    getchar();
    env_set_video_mode(m);
}

#endif
