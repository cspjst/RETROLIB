#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../doslib/src/DOS/dos_file_constants.h"
#include "../doslib/src/DOS/dos_file_services.h"
#include "../doslib/src/DOS/dos_error.h"
#include "../doslib/src/DOS/dos_error_codes.h"
#include "../doslib/bioslib/src/BIOS/bios_clock_services.h"

#include "CGA/cga_convert.h"
#include "MEM/dos_mem_arena.h"
#include "ENV/env_time.h"

#define DOS_MAX_PATH 128
#define MINARGS 1
#define ERROR "DOS errno %i %s\n"
#define USAGE "usage: ppm2cga <file.ppm> [file2.ppm ...]\n" \
              "       ppm2cga *.ppm\n"

int do_convert(const char* in_file, const char* out_file) {
    mem_arena_t* arena = mem_new_arena(4096);
    if(!arena) {
        dos_perror("mem_new_arena(4096) ", DOS_INSUFFICIENT_MEMORY);
        return 1;
    }
    bios_ticks_since_midnight_t t1, t2;
    bios_read_system_clock(&t1);
    if(!cga_convert_ppm_to_cga(in_file, out_file, arena)) {
        mem_free_arena(arena);
        return 1;
    }
    bios_read_system_clock(&t2);
    printf("conversion time = %fsec\n", env_ticks_to_seconds(t2 - t1));
    mem_free_arena(arena);
    return 0;
}

int do_wildcard(char* argv[]) {
    dos_dta_t dta;
    dos_dta_t* old_dta;
    char cgafile[DOS_MAX_PATH];
    char ppmfile[DOS_MAX_PATH];

    dos_get_dta(&old_dta);
    dos_set_dta(&dta);

    if(dos_find_first_file(argv[1], FIND_NORMAL, &dta)) {
        dos_perror(argv[1], DOS_FILE_NOT_FOUND);
        dos_set_dta(old_dta);
        return 1;
    }
    char* p = strrchr(argv[1], '\\');   // look for a preceding filepath
    if(p) *++p = '\0';                  // tokenize argv[1] as filepath
    else argv[1][0] = '\0';
    do {
        strcpy(ppmfile, argv[1]);
        strcpy(cgafile, argv[1]);
        strcat(ppmfile, dta.filename);
        strcat(cgafile, dta.filename);
        strcpy(strrchr(cgafile, '.'), ".CGA");
    } while(!do_convert(ppmfile, cgafile) && !dos_find_next_file());

    dos_set_dta(old_dta);
    return 0;
}

int main(int argc, char* argv[]) {
    switch(argc) {
        case 3:
            return do_convert(argv[1], argv[2]);
        case 2:
            return do_wildcard(argv);
        default:
            dos_perror("argc ", DOS_INVALID_FORMAT);
            fprintf(stderr, USAGE);
            return 1;
    }
}
