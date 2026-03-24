/**
 * @author      Jeremy Simon Thornton
 * @copyright   2024 Jeremy Simon Thornton
 * @version     0.1.0
 * @url https://www.stanislavs.org/helppc/
 *
 * The DOS INT 21,48 - Allocate Memory function takes a 16 bit word size request in paragraphs (16 bytes)
 * and returns a memory segment address as a 16 bit value.
 * Because the request is a 16 bit word the, *theoretical*, maximum number of paragraphs requestable to
 * allocate is 65535 paragraphs or 65535 x 16 = 1048560 bytes.
 * Which is 16 bytes, one paragraph, less than 1 Mebibyte (MiB)
 * @note 1MiB Max the original 8086/8088 microprocessors' 20-bit address bus can address 1,048,576 bytes.
 * However, the actual amount of allocatable RAM is restricted 640K or less (+ possible upper memory area)
 * and will depend on installed RAM.
 */
#include "dos_mem_arena.h"

#include <stddef.h>
#include <stdio.h>

#include "../../doslib/src/DOS/dos_memory_services.h"
#include "../../doslib/src/DOS/dos_memory_types.h"

#pragma pack(1)
typedef struct private_mem_arena {
    dos_address_t base; // base address of the arena (includes this struct)
    char*   begin;      // pointer to the start of arena after this struct
    char*   free;       // pointer to start of free memory, initially after this struct
    char*   end;        // end address limit of useable arena memory
} mem_arena_t;
#pragma pack()

// Allocate a DOS memory block and embed the arena struct at its start
mem_arena_t* mem_new_arena(dos_memsize_t paragraphs) {
	dos_address_t base = {0};
	printf("error = %i\n",dos_allocate_memory_blocks(
	    paragraphs + ((sizeof(mem_arena_t) + DOS_PARAGRAPH_SIZE - 1) / DOS_PARAGRAPH_SIZE),
		&base.segoff.segment
	));// != 0) return NULL;
	// success DOS could fulfill the memory request
	mem_arena_t* arena = (mem_arena_t*)base.ptr;
	arena->base = base;
	arena->begin = arena->free = (char*)arena->base.ptr + sizeof(mem_arena_t);
	arena->end = arena->begin + (paragraphs * DOS_PARAGRAPH_SIZE);
	return arena;
}

dos_memsize_t mem_arena_size(mem_arena_t* arena) {
	return arena->free - arena->begin;
}

dos_memsize_t mem_arena_capacity(mem_arena_t* arena) {
	return arena->end - arena->begin;
}

void* mem_arena_alloc(mem_arena_t* arena, dos_memsize_t byte_request) {
	char* p;
	if (byte_request > mem_arena_size(arena)) return NULL;  // unable fulfill request
	p = arena->free;					// initialize return value points to requested block
	arena->free += byte_request;		// shrink pool size
	return p;
}

dos_memsize_t mem_free_arena(mem_arena_t* arena) {
	dos_memsize_t freed = mem_arena_capacity(arena);
	if(dos_free_allocated_memory_blocks(arena->base.segoff.segment) != 0) return 0;
	arena->base.ptr = arena->begin = arena->free = arena->end = NULL;
	return freed;
}

char* mem_arena_mcb(mem_arena_t* arena) {
	dos_address_t m = arena->base;
	m.segoff.segment--;
	return m.ptr;
}
