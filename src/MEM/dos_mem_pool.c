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
#include "dos_mem_arena.h"

#include <stddef.h>

#include "../../doslib/src/DOS/dos_memory_types.h"
#include "../../doslib/src/DOS/dos_memory_services.h"
#include "../../doslib/src/DOS/dos_memory_constants.h"

#define HEADER_SIZE         4   // 64 bytes header = 4 paragraphs

#pragma pack(1)
typedef struct private_mem_arena {
    void* free;   // pointer to start of free memory, initially after this struct
    void* top;    // end address limit of useable arena memory
} mem_arena_t;            // 64 bytes
#pragma pack()

void* mem_arena_init(dos_memsize_t bytes) {
    mem_arena_t* mem = NULL;
    dos_address_t addr = {0};
    if (dos_allocate_memory_blocks(HEADER_SIZE + ((bytes + DOS_PARAGRAPH_SIZE - 1) / DOS_PARAGRAPH_SIZE), &addr.segoff.segment) == 0) {
        mem = (mem_arena_t*)addr.ptr;

    }

}
