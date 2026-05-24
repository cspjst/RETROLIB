#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../doslib/src/DOS/dos_file_services.h"
#include "../doslib/src/DOS/dos_error.h"
#include "../doslib/bioslib/src/BIOS/bios_clock_services.h"

#include "CGA/cga_convert.h"
#include "MEM/dos_mem_arena.h"
#include "ENV/env_time.h"

#define MINARGS 1
#define ERROR "DOS errno %i %s\n"
#define USAGE "usage: ppm2cga <file.ppm> [file2.ppm ...]\n" \
              "       ppm2cga *.ppm\n"


int main(int argc, char* argv[]) {
    bios_ticks_since_midnight_t t1, t2;
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) {
        dos_perror("mem_new_arena(4096) ", DOS_INSUFFICIENT_MEMORY);
        return 1;
    }
    switch(argc) {
        case 3:

            bios_read_system_clock(&t1);
            if(!cga_convert_ppm_to_cga(argv[1], argv[2], arena)) return 1;
            else {
                bios_read_system_clock(&t2);
                printf("conversion time = %fsec\n", env_ticks_to_seconds(t2-t1));
            }
            break;
        case 2:
        default:
            dos_perror("ERROR: ", DOS_INVALID_FORMAT);
            fprintf(stderr, USAGE);
            return 1;

    }
/*
    dos_dta_t dta;
    dos_dta_t** old_dta;



    //e = dos_find_first_file(const char* filespec, dos_file_attributes_t attributes, dos_dta_t* dta);

    e = dos_get_dta(old_dta);
    if(e) goto error;
    dos_set_dta(&dta);



    dos_set_dta(*old_dta);
*/
    mem_free_arena(arena);
    return 0;

}
