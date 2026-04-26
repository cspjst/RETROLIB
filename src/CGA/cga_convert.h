#ifndef CGA_CONVERT_H
#define CGA_CONVERT_H

#include "../MEM/dos_mem_arena.h"

dos_memsize_t cga_convert_ppm_to_raw(
    const char* file_in_path,
    const char* file_out_path,
    mem_arena_t* arena
);

#endif
