#include "cga_hi_res_cls.h"

#include "cga_constants.h"

void __fastcall cga_hi_res_cls(cga_colour_t colour) {
    __asm {
        .8086
        push    es
        pushf

        mov     ah, al                      ; duplicate colour byte
        les     di, CGA_VRAM_PTR            ; ES:DI points to start VRAM
        mov     cx, CGA_SCREEN_WORDS        ; 2000h words
        cld                                 ; increment DI
        rep     stosw                       ; store AX all VRAM

        popf
        pop     es
    }
}
