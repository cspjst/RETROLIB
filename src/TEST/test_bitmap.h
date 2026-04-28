#ifndef TEST_BITMAP_H
#define TEST_BITMAP_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "../MEM/dos_mem_arena.h"

#include "../CGA/cga_bitmap.h"
#include "../CGA/cga_convert.h"

#include "../ENV/env_time.h"

#include "../../doslib/bioslib/src/BIOS/bios_clock_services.h"

void test_dump_data(char** src, int w, int h) {
    for(int y = 0; y <h; ++y) {
        for(int x = 0; y < w; y++) {
            printf("%02X ", src[y][x]);
        }
        putchar('\n');
    }
}

void test_hi_res_convert() {
    bios_ticks_since_midnight_t t1, t2;
    printf("Testing CGA hi res convert PPM to CGA bitmap\n");
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");
    printf("converting PBM to CGA...\n");

    bios_read_system_clock(&t1);
    if(!cga_convert_pbm_to_raw("../res/joker.pbm", "../res/joker.cga", arena))
        printf("\tconversion error %s\n", strerror(errno));
    else {
        bios_read_system_clock(&t2);
        printf("conversion time = %fsec\n", env_ticks_to_seconds(t2-t1));
    }

    mem_free_arena(arena);
}

void test_hi_res_bitmap() {
    printf("Testing CGA hi res \"joker.cga\" bitmap\n");
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");

    cga_bitmap_t* bmp = cga_bmp_load("../res/joker.cga", arena);
    if(!bmp) printf("error %s\n", strerror(errno));

    cga_bmp_dump(stdout, bmp);

    mem_free_arena(arena);
}

void test_lo_res_convert() {
    bios_ticks_since_midnight_t t1, t2;
    printf("Testing CGA lo res convert PPM to CGA bitmap\n");
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");
    printf("converting PPM to CGA...\n");

    bios_read_system_clock(&t1);
    if(!cga_convert_ppm_to_raw("../res/tree.ppm", "../res/tree.cga", arena))
        printf("\tconversion error %s\n", strerror(errno));
    else {
        bios_read_system_clock(&t2);
        printf("conversion time = %fsec\n", env_ticks_to_seconds(t2-t1));
    }

    mem_free_arena(arena);
}

void test_lo_res_bitmap() {
    printf("Testing CGA lo res \"tree.cga\" bitmap\n");
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");

    cga_bitmap_t* bmp = cga_bmp_load("../res/tree.cga", arena);
    if(!bmp) printf("error %s\n", strerror(errno));

    cga_bmp_dump(stdout, bmp);

    mem_free_arena(arena);
}

void test_shift() {
    char src[4][4] = {  {0xF1, 0x55, 0x55, 0x0F},
                        {0xF2, 0x55, 0x55, 0x0F},
                        {0xF3, 0x55, 0x55, 0x0F},
                        {0xF4, 0x55, 0x55, 0x0F} };

    char dst[4][4] = {0};




    putchar('\n');


}

void test_bitmap() {
    //test_hi_res_convert();
    //test_hi_res_bitmap();
    //test_lo_res_convert();
    //test_lo_res_bitmap();
    //test_shift();
}

#endif
