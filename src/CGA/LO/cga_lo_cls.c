#include "cga_lo_cls.h"
#include "../cga_constants.h"

void cga_lo_cls() {
    __asm {
        .8086
        push    es
        pushf

        cld                                 ; incremental STOSW
        mov     di, CGA_VIDEO_RAM_SEGMENT
        mov     es, di
        xor     di, di
        mov     cx, CGA_SCREEN_WORDS        ; 2000h words
        xor     ax, ax                      ; black
        rep     stosw                       ; store AX full screen VRAM

        popf
        pop     es
    }
}

void __fastcall cga_lo_colour_cls(cga_lo_res_colour_t colour) {
    __asm {
        .8086
        push    es
        pushf

        mov     ah, al                      ; duplicate colour pixel
        shr     al, 1                       ; shift pixel right
        shr     al, 1
        or      ah, al
        shr     al, 1                       ; shift pixel right
        shr     al, 1
        or      ah, al
        shr     al, 1                       ; shift pixel right
        shr     al, 1
        or      ah, al
        mov     al, ah

        cld                                 ; incremental STOSW
        mov     di, CGA_VIDEO_RAM_SEGMENT
        mov     es, di
        xor     di, di
        mov     cx, CGA_SCREEN_WORDS        ; 2000h words

        rep     stosw                       ; store AX full screen VRAM

        popf
        pop     es
    }
}
