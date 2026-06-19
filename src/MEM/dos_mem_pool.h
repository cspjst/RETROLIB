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
#ifndef DOS_MEM_ARENA_H
#define DOS_MEM_ARENA_H

#include "../../doslib/src/DOS/dos_memory_types.h"

char* mem_pool_init(dos_memsize_t capacity);

dos_memsize_t mem_pool_size(char* pool);

void mem_pool_free(char* pool);

#endif
