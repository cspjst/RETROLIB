#ifndef TEST_ARENA_H
#define TEST_ARENA_H

#include <stdio.h>
#include <assert.h>

#include "../MEM/dos_mem_arena.h"
#include "../../doslib/src/DOS/dos_memory_services.h"

void test_new() {
    printf("Testing  mem_new_arena(64)... ");
    mem_arena_t* arena = mem_new_arena(64);
    assert(arena != NULL);
    assert(mem_arena_capacity(arena) == 64 * DOS_PARAGRAPH_SIZE);
    assert(mem_arena_size(arena) == 0);
    dos_memsize_t n = mem_free_arena(arena);
    assert( n == 64 * DOS_PARAGRAPH_SIZE);
    assert(mem_free_arena(arena) == 0);
    assert(mem_free_arena(arena) == 0);
    assert(mem_free_arena(arena) == 0);

    arena = mem_new_arena(0);
    assert(arena != NULL);
    assert(mem_arena_capacity(arena) == 0);
    assert(mem_arena_alloc(arena, 1) == NULL);
    assert(mem_free_arena(arena) == 0);

    arena = mem_new_arena(DOS_MAX_ALLOCATE_BYTES);
    assert(arena == NULL);
    assert(mem_free_arena(arena) == 0);

    printf("PASS\n");
}

void test_alloc() {
    printf("Testing mem_arena_alloc... ");
    mem_arena_t* arena = mem_new_arena(128);
    assert(arena != NULL);
    dos_memsize_t cap = mem_arena_capacity(arena);
    assert(cap == (128 * DOS_PARAGRAPH_SIZE));
    assert(mem_arena_size(arena) == cap);

    void* ptr1 = mem_arena_alloc(arena, 100);
    void* ptr2 = mem_arena_alloc(arena, 256);
    void* ptr3 = mem_arena_alloc(arena, 50);

    assert(ptr1 != NULL);
    assert(ptr2 != NULL);
    assert(ptr3 != NULL);
    assert((char*)ptr2 == (char*)ptr1 + 100);
    assert((char*)ptr3 == (char*)ptr2 + 256);

    assert(mem_arena_alloc(arena, 99999) == NULL);

    void* ptr = mem_arena_alloc(arena, 0);
    assert(ptr != NULL);    // Zero-byte alloc should succeed

    assert(mem_free_arena(arena) == 128 * DOS_PARAGRAPH_SIZE);

    arena = mem_new_arena(64);
    assert(arena != NULL);
    cap = mem_arena_capacity(arena); // exact capacity allocation
    void* p = mem_arena_alloc(arena, cap);
    assert(p != NULL);
    assert(mem_arena_size(arena) == 0);
    assert(mem_arena_alloc(arena, 1) == NULL); // Next allocation should fail

    assert(mem_free_arena(arena) == 64 * DOS_PARAGRAPH_SIZE);

    printf("PASS\n");
}

void test_exhaustion() {
    printf("Testing arena exhaustion... ");

    mem_arena_t* arena = mem_new_arena(64);
    assert(arena != NULL);

    dos_memsize_t cap = mem_arena_capacity(arena);
    dos_memsize_t allocated = 0;

    while(1) { // Allocate in small chunks until exhausted
        void* ptr = mem_arena_alloc(arena, 1);
        if(ptr == NULL) break;
        allocated ++;
    }

    assert(allocated == cap);
    assert(mem_arena_size(arena) == 0);

    assert(mem_free_arena(arena) == 64 * DOS_PARAGRAPH_SIZE);

    printf("PASS\n");
}

void test_mcb(void) {
    printf("Testing mem_arena_mcb... ");

    mem_arena_t* arena = mem_new_arena(64);
    assert(arena != NULL);

    char* mcb = mem_arena_mcb(arena);

    mem_free_arena(arena);

    printf("PASS\n");
}

void test_order(void) {
    printf("Testing pointer ordering... ");

    mem_arena_t* arena = mem_new_arena(128);
    assert(arena != NULL);

    void* p1 = mem_arena_alloc(arena, 10);
    void* p2 = mem_arena_alloc(arena, 20);
    void* p3 = mem_arena_alloc(arena, 30);

    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(p3 != NULL);
    assert(p1 < p2);
    assert(p2 < p3);
    assert((char*)p2 == (char*)p1 + 10);
    assert((char*)p3 == (char*)p2 + 20);

    printf("PASS\n");

    mem_free_arena(arena);
}

void test_arena() {
    test_new();
}

#endif
