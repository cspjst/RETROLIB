/**
 * Loads a cga file and then using the first image as originator
 * pre-shifts the remaining 3 bitmap data bloxk for lo res mode 4
 * at 2 bits-per-pixel in order to optimise block transfer to VRAM by
 * providing pre-shifted bitmaps at sub-byte x mod 4 positions 0..3
 * and so trading RAM for performance.
 */
#include <stdio.h>
#include <string.h>

#include "../doslib/src/DOS/dos_file_constants.h"
#include "../doslib/src/DOS/dos_file_services.h"
#include "../doslib/src/DOS/dos_file_tools.h"
#include "../doslib/src/DOS/dos_error.h"
#include "../doslib/src/DOS/dos_error_codes.h"

#include "MEM/dos_mem_arena.h"

#include "CGA/cga_bitmap.h"
#include "CGA/LO/cga_lo_scroll.h"

#define USAGE "usage: cgashift <file.cga> [file2.cga ...]\n" \
              "       cgashift *.cga\n"

int do_shift(const char* cga_file) {
    mem_arena_t* arena = mem_new_arena();
    if(!arena) return 1;
    cga_bitmap_t* bmp = cga_bmp_load(cga_file, arena);
    if(!bmp) return 1;
    bmp->data[1] = (char*)mem_arena_alloc(arena, bmp->size);
    if(!bmp->data[1]) return 1;
    cga_lo_scroll_right(bmp->data[0], bmp->data[1], bmp->width, bmp->height, 0, 0);
    bmp->data[2] = (char*)mem_arena_alloc(arena, bmp->size);
    if(!bmp->data[2]) return 1;
    cga_lo_scroll_right(bmp->data[1], bmp->data[2], bmp->width, bmp->height, 0, 0);
    bmp->data[3] = (char*)mem_arena_alloc(arena, bmp->size);
    if(!bmp->data[3]) return 1;
    cga_lo_scroll_right(bmp->data[2], bmp->data[3], bmp->width, bmp->height, 0, 0);
    cga_bmp_dump(stderr, bmp);
    //if(cga_bmp_save(cga_file, bmp) == 0) return 1;
    return 0;
}

int do_wildcard(char* argv[]) {
    dos_dta_t dta;
    dos_dta_t* old_dta;
    char cgafile[DOS_MAX_PATH];

    dos_get_dta(&old_dta);
    dos_set_dta(&dta);

    if(dos_find_first_file(argv[1], FIND_NORMAL, &dta) != 0) {
        dos_perror(argv[1], DOS_FILE_NOT_FOUND);
        dos_set_dta(old_dta);
        return 1;
    }
    char* p = strrchr(argv[1], '\\');   // look for a preceding filepath
    if(p) *++p = '\0';                  // tokenize argv[1] as filepath
    else argv[1][0] = '\0';
    do {
        strcpy(cgafile, argv[1]);
        strcat(cgafile, dta.filename);
    } while(!do_shift(cgafile) && !dos_find_next_file());

    dos_set_dta(old_dta);
    return 0;
}

int main(int argc, char* argv[]) {
    switch(argc) {
        case 1:
            dos_perror("argc ", DOS_INVALID_FORMAT);
            fprintf(stderr, USAGE);
            return 1;
        case 2:
            if(strlen(argv[1]) > DOS_MAX_PATH) { dos_perror("argv[1] > DOS_MAX_PATH ", DOS_INVALID_FORMAT); return 1; }
            return do_wildcard(argv);
        default:
            for(int i = 1; i <= argc; ++i) {
                if(strlen(argv[i]) > DOS_MAX_PATH) { dos_perror("argv[] > DOS_MAX_PATH ", DOS_INVALID_FORMAT); return 1; }
                do_shift(argv[i]);
            }
    }
}
