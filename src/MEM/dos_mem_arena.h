/**
 *  @author      Jeremy Simon Thornton
 *  @copyright   2024 Jeremy Simon Thornton
 *  @version     0.1.2
 *  @url https://www.stanislavs.org/helppc/
 *  @brief     The memory arena paradigm - one lifetime to bind them all.
 *  @details   Untangling lifetimes with absurdly simple linear allocator, lightning fast allocation and
 *  deallocation, eliminating lifetime soup of per-allocation freeing/stack fragility/garbage collection requirements.
 *  Memory Arena the area where dynamic runtime memory is stored.
 *  The memory arena consists of the heap and unused memory.
 *  The heap is where all user-allocated memory is located.
 *  The heap grows up from a lower memory address to a higher memory address.
 *  synonyms: region, zone, arena, area, or memory context.
 *  @note *By design* unable to free individual allocated memory blocks *only* the whole backing memory block arena
 *  @url https://www.youtube.com/watch?v=3IAlJSIjvH0&t=4s
 */
#ifndef MEM_ARENA_H
#define MEM_ARENA_H

#include "../../doslib/src/DOS/dos_memory_types.h"

typedef struct private_mem_arena mem_arena_t;

mem_arena_t* mem_new_arena(dos_memsize_t paragraphs);

dos_memsize_t mem_arena_size(mem_arena_t* arena);

dos_memsize_t mem_arena_capacity(mem_arena_t* arena);

void* mem_arena_alloc(mem_arena_t* arena, dos_memsize_t byte_request);

dos_memsize_t mem_free_arena(mem_arena_t* arena);

dos_mcb_t* mem_arena_mcb(mem_arena_t* arena);

#endif
