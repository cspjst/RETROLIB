#ifndef TEST_POOL_H
#define TEST_POOL_H

#include <stdio.h>
#include <assert.h>

#include "../MEM/dos_mem_pool.h"

//#include "../../doslib/src/DOS/dos_memory_constants.h"

void test_init() {
    printf("Testing arena init & free...\n");
    dos_memsize_t pcap =64;

    char* pool = mem_pool_init(pcap);
    assert(pool != NULL);

    assert(mem_pool_size(pool) == pcap);

    mem_pool_free(pool);
    assert(mem_pool_size(pool) == 0);

    pool = mem_pool_init(0);
    assert(pool == NULL);
    //assert(mem_arena_capacity(arena) == 0);
    //assert(mem_arena_alloc(arena, 1) == NULL);
    //assert(mem_free_arena(arena) == 0);

    //arena = mem_new_arena(DOS_MAX_ALLOCATE_BYTES);
    //assert(arena == NULL);
    //assert(mem_free_arena(arena) == 0);

    printf("PASS\n");
}

void test_pool() {
    test_init();
}

#endif
