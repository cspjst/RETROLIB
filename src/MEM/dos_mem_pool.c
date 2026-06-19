/**
 * @author      Jeremy Simon Thornton
 * @copyright   2024 Jeremy Simon Thornton
 *
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
#include "dos_mem_pool.h"

#include <stddef.h>

#include "../../doslib/src/DOS/dos_memory_types.h"
#include "../../doslib/src/DOS/dos_memory_services.h"
#include "../../doslib/src/DOS/dos_memory_constants.h"

#include <stdio.h>

#pragma pack(1)
typedef struct private_mem_arena {
    char* free;   // pointer to start of free memory, initially after this struct
    char* top;    // end address limit of useable arena memory
} mem_pool_t;
#pragma pack()

char* mem_pool_init(dos_memsize_t capacity) {
    if(!capacity) return NULL;
    dos_memsize_t paragraphs = (sizeof(mem_pool_t) + capacity + DOS_PARAGRAPH_SIZE - 1) / DOS_PARAGRAPH_SIZE;
    dos_address_t addr = {0};
    if (dos_allocate_memory_blocks(paragraphs, &addr.segoff.segment) == 0) {
        mem_pool_t* pool = (mem_pool_t*)addr.ptr;
        pool->free = (char*)(pool + 1);
        pool->top = pool->free + capacity;
        return (char*)(pool + 1);
    }
    return NULL;
}

dos_memsize_t mem_pool_size(char* pool) {
    if(!pool) return 0;
    mem_pool_t* p = ((mem_pool_t*)pool) - 1;
    return p->top - p->free;
}

void mem_pool_free(char* pool) {
    dos_address_t addr = {0};
    mem_pool_t* p = ((mem_pool_t*)pool) - 1;
    addr.ptr = p;
    if(dos_free_allocated_memory_blocks(addr.segoff.segment) == 0) p->free = p->top = NULL;
}
