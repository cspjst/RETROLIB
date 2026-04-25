#ifndef TEST_BITMAP_H
#define TEST_BITMAP_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "../MEM/dos_mem_arena.h"

#include "../CGA/cga_bitmap.h"

void test_hi_res_bitmap() {
    printf("Testing CGA hi res \"joker.pbm\" bitmap\n");
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");

    cga_bitmap_t bmp = {0};

    cga_bmp_dump(stdout, &bmp);

    mem_free_arena(arena);
}

void test_lo_res_bitmap() {
    printf("Testing CGA lo res \"tree.ppm\" bitmap\n");
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");

    cga_bitmap_t bmp = {0};

    cga_bmp_dump(stdout, &bmp);

    mem_free_arena(arena);
}

void test_bitmap() {
    test_hi_res_bitmap();
    test_lo_res_bitmap();
}

#endif
