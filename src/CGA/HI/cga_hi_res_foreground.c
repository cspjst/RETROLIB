#include "cga_hi_res_foreground.h"
#include "../cga_constants.h"

void cga_hi_res_set_fg(cga_palette_colour_t colour) {
    __asm {
        .8086
        push    ds
        pushf

        mov     dx, CGA_PALETTE_REG     ; 0x3D9
        in      al, dx                  ; Read current value
        and     al, 0F0h                ; Preserve bits 4-7, clear bits 0-3
        or      al, colour              ; Set new foreground (bits 0-3)
        out     dx, al                  ; Write back

        popf
        pop     ds
    }
}
