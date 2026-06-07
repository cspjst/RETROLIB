#include <stdio.h>
#include "MEM/dos_mem_dump.h"

int main(int argc, char* argv[]) {

    char* rom = (char*)0xF000E000;
    int nchars = 60;
    dos_mem_dump_block(stdout, rom, rom + nchars);

    return 0;
}

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
