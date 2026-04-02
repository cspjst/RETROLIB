#include "cga_hi_res_blt.h"
#include "cga_constants.h"

void cga_hi_res_fill_vram(const char* data) {
    __asm {
        .8086
        push    ds
        push    es
        pushf

        lds     si, data                    ; DS:SI source RAM
        les     di, CGA_VRAM_PTR            ; ES:DI destination VRAM
        mov     cx, CGA_SCREEN_WORDS        ; 2000h words
        cld                                 ; increment DI
        rep     movsw                       ; copy 16K to VRAM

        popf
        pop     es
        pop     ds
    }
}
