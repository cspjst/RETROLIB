#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "MEM/dos_mem_dump.h"
#include "LIB/lib_convert.h"

#include "../doslib/src/DOS/dos_error.h"
#include "../doslib/src/DOS/dos_error_codes.h"
#include "../doslib/src/DOS/dos_memory_constants.h"
#include "../doslib/src/DOS/dos_memory_types.h"

#include "../doslib/bioslib/src/BIOS/bios_video_services.h"
#include "../doslib/bioslib/src/BIOS/bios_keyboard_constants.h"
#include "../doslib/bioslib/src/BIOS/bios_keyboard_services.h"
#include "../doslib/bioslib/src/BIOS/bios_keyboard_scan_codes.h"

#define WIDTH       79
#define HEIGHT      24
#define USAGE "usage: memdump <segment> <offset>\n" \
              "       e.g. memdump F000 E000\n" \
              "       segment and offset are 4-digit hexadecimal\n"

int mem_dump(dos_address_t addr) {
    bool is_valid_key;
    bios_key_t key;
    bios_keybd_info_t flags;
    int step = 0;                   // scroll and window step
    char* begin = (char*)addr.ptr;  // setup memory window full screen i.e. 448 bytes
    char* end = begin + (DOS_PARAGRAPH_SIZE * HEIGHT);
    char* from = begin;             // setup dump window, initially = memory window
    char* to = end;

    bios_scroll_active_page_down(0, BIOS_FG_WHITE || BIOS_BG_BLACK, 0, 0, WIDTH, HEIGHT);  // clear page
    bios_set_cursor_position(0, HEIGHT + 1); // bottom left 
    printf                      // print key controls
    bios_set_cursor_position(0, 0); // top left 
    while(true) {
        is_valid_key = false;
        dos_mem_dump_block(stdout, from, to);
        while(!is_valid_key) {
            bios_wait_for_keystroke_and_read(&key);  // key press
            bios_get_keyboard_flags(&flags);         // modifier keys
            // scroll by one line (16 bytes 1 paragraph) unless shift key modify to 4 lines (4 paragraphs)
            step = (flags & (BIOS_KEY_LEFT_SHIFT | BIOS_KEY_RIGHT_SHIFT)) ? 64 : 16;
            switch(key.parts.scan) {
                case SCAN_ESC: return 0;             // ESC to exit 
                case SCAN_DOWN:
                    is_valid_key = true;
                    bios_scroll_active_page_down(step / 16, BIOS_FG_WHITE || BIOS_BG_BLACK, 0, 0, WIDTH, HEIGHT);
                    bios_set_cursor_position(0, 0);  // top left
                    begin += step;                   // move memory window
                    end += step;                     
                    from = begin;                    // dump top section memory window
                    to = from + step;
                    break;
                case SCAN_UP:
                    is_valid_key = true;
                    bios_scroll_active_page_up(step / 16, BIOS_FG_WHITE || BIOS_BG_BLACK, 0, 0, WIDTH, HEIGHT);
                    bios_set_cursor_position(0, HEIGHT);  // bottom right 
                    from = end;                      // dump lower section memory window 
                    to = from + step;
                    begin += step;                   // move memory window
                    end += step;
                    break;
            }  // not a valid key
        } // valid key so dump missing section of memory window (from, to) 
    } // loop until ESC pressed 
}

int main(int argc, char* argv[]) {
    switch(argc) {
        case 3:
            if(lib_str_is_hex(argv[1]) && lib_str_is_hex(argv[2])) {
                dos_address_t addr;
                addr.segoff.segment = (unsigned short)strtoul(argv[1], NULL, 16);
                addr.segoff.offset  = (unsigned short)strtoul(argv[2], NULL, 16);
                return mem_dump(addr);
            }
            // fallthrough
        default:
            dos_perror(argv[argc - 1], DOS_INVALID_FORMAT);
            fprintf(stderr, USAGE);
            return 1;
    }
}
