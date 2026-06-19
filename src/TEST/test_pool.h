#ifndef TEST_POOL_H
#define TEST_POOL_H

#include <stdio.h>
#include <assert.h>

#include "../MEM/dos_mem_pool.h"

void test_init() {
    printf("Testing arena init & free...\n");
    dos_memsize_t cap =64;

    char* pool = dos_mem_pool_init(cap);
    assert(pool != NULL);

    assert(dos_mem_pool_size(pool) == cap);

    dos_mem_pool_free(pool);
    assert(dos_mem_pool_size(pool) == 0);

    pool = dos_mem_pool_init(0);
    assert(pool == NULL);
    assert(dos_mem_pool_size(pool) == 0);
    assert(dos_mem_pool_alloc(pool, 0) == NULL);
    assert(dos_mem_pool_alloc(pool, 1) == NULL);
    dos_mem_pool_free(pool);

    pool = dos_mem_pool_init(100);//dos_mem_1MB);
    assert(pool == NULL);
    assert(dos_mem_pool_size(pool) == 0);
    assert(dos_mem_pool_alloc(pool, 0) == NULL);
    assert(dos_mem_pool_alloc(pool, 1) == NULL);
    dos_mem_pool_free(pool);

    printf("PASS\n");
}

void test_pool() {
    test_init();
}

#endif
