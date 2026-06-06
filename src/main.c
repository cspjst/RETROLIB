#include <stdio.h>
#include <string.h>

#include "../doslib/src/DOS/dos_file_constants.h"
#include "../doslib/src/DOS/dos_error.h"
#include "../doslib/src/DOS/dos_error_codes.h"

#include "CGA/LO/cga_lo_blit.h"
#include "CGA/cga_bitmap.h"
#include "CGA/LO/cga_lo_cls.h"
#include "CGA/cga_colours.h"
#include "ENV/env_video_mode.h"
#include "MEM/dos_mem_arena.h"

#define USAGE "usage: cgashow <file.cga>\n"

int do_show(const char* cga_file) {
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) return 1;
    cga_bitmap_t* bmp = cga_bmp_load(cga_file, arena);
    if(!bmp) goto fail;
    cga_bmp_dump(stdout, bmp);
    printf("\nChange to mode %i and display image block(s) %s ? y/n\n", bmp->depth == 1 ? 6 : 4, cga_file);
    if(getchar() != 'y') {
        mem_free_arena(arena);
        return 0;
    }
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_GRAY_320X200);
    cga_lo_colour_cls(CGA_LO_RES_CYAN);
    printf("Press enter to display each avialable \nbitmap data block...\n");
    getchar();
    getchar();

    cga_lo_cls();
    cga_lo_blit_paste(0, 0, bmp->width, bmp->height, bmp->data[0]);
    getchar();

    env_set_video_mode(m);
    mem_free_arena(arena);
    return 0;
fail:
    mem_free_arena(arena);
    return 1;
}

int main(int argc, char* argv[]) {
    switch(argc) {
        case 2:
            if(strlen(argv[1]) > DOS_MAX_PATH) { dos_perror("argv[1] > DOS_MAX_PATH ", DOS_INVALID_FORMAT); return 1; }
            return do_show(argv[1]);
        default:
            dos_perror("argc ", DOS_INVALID_FORMAT);
            fprintf(stderr, USAGE);
            return 1;
    }
}
