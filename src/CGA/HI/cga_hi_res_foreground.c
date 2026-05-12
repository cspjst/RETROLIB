#include "cga_hi_res_foreground.h"
#include "../cga_constants.h"

void cga_hi_set_foreground_colour(cga_palette_colour_t colour) {
    __asm {
        .8086
        push    es

        mov     cl, colour
        mov     ax, 40h                 ; BDA segment
        mov     es, ax                  ; preserve DS
        mov     al, es:[66h]            ; read BDA palette shadow
        and     al, 0F0h                ; preserve bits 7..4
        or      al, cl                  ; merge foreground colour
        mov     es:[66h], al            ; update BDA shadow
        mov     dx, CGA_PALETTE_REG     ; port 3D9h
        out     dx, al

        pop     es
    }
}
