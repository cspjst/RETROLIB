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

#define MEM_TOP     0xF000FFFFULL
#define SCREEN_TOP  0xF000FE7FUL
#define WIDTH       79
#define HEIGHT      24
#define SHIFT_STEP  12
#define CHR_VBAR     "\xB3"   /* │ */
#define CHR_UARROW   "\x18"   /* ↑ */
#define CHR_DARROW   "\x19"   /* ↓ */

#define USAGE "usage: memdump <segment> <offset>\n" \
              "       e.g. memdump F000 E000\n" \
              "       segment and offset are 4-digit hexadecimal\n"

int mem_dump(dos_address_t addr) {
    bool is_valid_key;
    bios_key_t key;
    bios_keybd_info_t flags;
    int step = DOS_PARAGRAPH_SIZE * HEIGHT; // scroll and window step
    char* begin = (char*)addr.ptr;  // setup memory window full screen i.e. 448 bytes
    if((unsigned long long)begin + step > MEM_TOP) step = (int)(MEM_TOP - (unsigned long long)begin);
    char* end = begin + step;
    char* from = begin;             // setup dump window, initially = memory window
    char* to = end;

    bios_scroll_active_page_down(0, BIOS_FG_WHITE | BIOS_BG_BLACK, 0, 0, WIDTH, HEIGHT);  // clear page
    bios_set_cursor_position(0, HEIGHT); // bottom left
    printf("\tESC=Quit " CHR_VBAR " " CHR_UARROW CHR_DARROW "=Scroll 1 line " CHR_VBAR " " "Shift+" CHR_UARROW CHR_DARROW "=Scroll 12 lines");                     // print key controls
    fflush(stdout);
    bios_set_cursor_position(0, 0); // top left
    while(true) {
        is_valid_key = false;
        dos_mem_dump_block(stdout, from, to);
        while(!is_valid_key) {
            bios_wait_for_keystroke_and_read(&key);  // key press
            bios_get_keyboard_flags(&flags);         // modifier keys
            // scroll by one line (16 bytes 1 paragraph) unless shift key modify to 12 lines (12 paragraphs)
            step = (flags & (BIOS_KEY_LEFT_SHIFT | BIOS_KEY_RIGHT_SHIFT)) ? SHIFT_STEP * DOS_PARAGRAPH_SIZE : DOS_PARAGRAPH_SIZE;
            switch(key.parts.scan) {
                case SCAN_ESC:
                    bios_scroll_active_page_down(0, BIOS_FG_LIGHT_GREY | BIOS_BG_BLACK, 0, 0, WIDTH, HEIGHT);  // clear page
                    bios_set_cursor_position(0, 0); // top left
                    return 0;             // ESC to exit
                case SCAN_UP:
                    if ((unsigned long)begin < (unsigned long)step) step = (int)(unsigned long)begin;   // clamp step to what's left
                    if (step == 0) break;            // already at base
                    begin -= step;                   // move memory window
                    end -= step;
                    from = begin;                    // dump top section memory window
                    to = from + step;
                    bios_scroll_active_page_down(step / 16, BIOS_FG_WHITE | BIOS_BG_BLACK, 0, 0, WIDTH, HEIGHT - 1);
                    bios_set_cursor_position(0, 0);  // top left
                    is_valid_key = true;
                    break;
                case SCAN_DOWN:
                    if ((unsigned long long)end + step > MEM_TOP) step = (int)(MEM_TOP - (unsigned long long)end);
                    if (step <= 0) break;
                    from = end;                      // dump lower section memory window
                    to = from + step;
                    begin += step;                   // move memory window
                    end += step;
                    is_valid_key = true;
                    bios_scroll_active_page_up(step / 16, BIOS_FG_WHITE | BIOS_BG_BLACK, 0, 0, WIDTH, HEIGHT - 1);
                    bios_set_cursor_position(0, HEIGHT - step /16);  // bottom left
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
                if ((unsigned long)addr.ptr >= MEM_TOP) {
                    fprintf(stderr, "error: address exceeds 1MB limit F000:FFFF\n");
                    fprintf(stderr, USAGE);
                    return 1;
                }
                return mem_dump(addr);
            }
            // fallthrough
        default:
            dos_perror(argv[argc - 1], DOS_INVALID_FORMAT);
            fprintf(stderr, USAGE);
            return 1;
    }
}
