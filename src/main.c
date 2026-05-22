#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "MEM/dos_mem_arena.h"
#include "ENV/env_time.h"
#include "CGA/cga_convert.h"
#include "../doslib/bioslib/src/BIOS/bios_clock_services.h"

#define NARGS 2
#define ERROR "error: at least 1 input filename required\n"
#define USAGE "usage: ppm2cga <file.ppm> [file2.ppm ...]\n" \
              "       ppm2cga *.ppm\n"

int main(int argc, char* argv[]) {
    if(argc < NARGS) {
        fprintf(stderr, ERROR);
        fprintf(stderr, USAGE);
        return 1;
    }

    for(int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    return 0;
    /*
    bios_ticks_since_midnight_t t1, t2;
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");
    printf("Converting %s to %s\n", argv[1], argv[2]);


    bios_read_system_clock(&t1);
    if(!cga_convert_ppm_to_cga(argv[1], argv[2], arena))
      printf("\tconversion error %s\n", strerror(errno));
    else {
      bios_read_system_clock(&t2);
      printf("conversion time = %fsec\n", env_ticks_to_seconds(t2-t1));
    }

    mem_free_arena(arena);
    */
}
