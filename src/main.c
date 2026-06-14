#include <stdio.h>
#include <string.h>

#include "MEM/dos_mem_dump.h"

#include "../doslib/src/DOS/dos_error.h"
#include "../doslib/src/DOS/dos_error_codes.h"
#include "../doslib/src/DOS/dos_memory_constants.h"

#include "../doslib/bioslib/src/BIOS/bios_video_services.h"
#include "../doslib/bioslib/src/BIOS/bios_keyboard_services.h"
#include "../doslib/bioslib/src/BIOS/bios_keyboard_scan_codes.h"

#define VALID_SIZE  9   // ABCD:1234
#define WIDTH       79
#define HEIGHT      24
#define USAGE "usage: memdump <segment:offset>\n" \
              "       e.g. memdump F000:E000\n" \
              "       segment and offset are 4-digit hexadecimal\n"

int mem_dump(char* addr) {
    int valid;
    bios_key_t key;

    while(1) {
        bios_scroll_active_page_down(0, BIOS_FG_WHITE || BIOS_BG_BLACK, 0, 0, WIDTH, HEIGHT);
        bios_set_cursor_position(0, 0);
        dos_mem_dump_block(stdout, (char*)0xF000E000, (char*)0xF000E000 + (DOS_PARAGRAPH_SIZE * HEIGHT));
        valid = 0;
        while(!valid) {
            bios_wait_for_keystroke_and_read(&key);
            switch(key.parts.scan) {
                case SCAN_ESC: return 0;
                case SCAN_DOWN:
                    valid = 1;
                    break;
                case SCAN_UP:
                    valid = 1;
                    break;
            }
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    switch(argc) {
        case 2:
            if(strlen(argv[1]) == VALID_SIZE && argv[1][4] == ':') {
                return mem_dump(argv[1]);
            }
        default:
            dos_perror(argv[argc - 1], DOS_INVALID_FORMAT);
            fprintf(stderr, USAGE);
            return 1;
    }

    return 0;
}

/*
char* rom = (char*)0xF000E000;
int nchars = 60;
dos_mem_dump_block(stdout, rom, rom + nchars);
*/
/*
    int main(int argc, char* argv[]) {
        char* begin   = (char*)strtoul(argv[1], NULL, 16);
        char* end     = begin + atoi(argv[2]);
        char* current = begin;
        bios_key_t key;
        bios_keybd_info_t flags;
        int step;

        if(end - begin <= VIEWER_STRIDE) {
            dos_mem_dump_block(stdout, begin, end);
            return 0;
        }

        while(1) {
            bios_scroll_active_page_up(0, BIOS_FG_LIGHT_GREY | BIOS_BG_BLACK, 0, 0, 79, 24);
            bios_set_cursor_position(0, 0);
            dos_mem_dump_block(stdout, current, current + VIEWER_STRIDE);

            bios_wait_for_keystroke_and_read(&key);
            bios_get_keyboard_flags(&flags);
            step = (flags & (BIOS_KEY_LEFT_SHIFT | BIOS_KEY_RIGHT_SHIFT)) ? 64 : 16;

            switch(key.parts.scan) {
                case SCAN_ESC: return 0;
                case SCAN_DOWN:
                    if(current + step < end - VIEWER_STRIDE) current += step;
                    break;
                case SCAN_UP:
                    if(current - step >= begin) current -= step;
                    break;
            }
        }
    }
 */
