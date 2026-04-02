/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#include "cga_hi_res_cls.h"
#include "cga_constants.h"

void __fastcall cga_hi_res_cls(cga_colour_t pattern) {
    __asm {
        .8086
        push    es
        pushf

        mov     ah, al                      ; duplicate pattern byte
        les     di, CGA_VRAM_PTR            ; ES:DI points to start VRAM
        mov     cx, CGA_SCREEN_WORDS        ; 2000h words
        cld                                 ; increment DI
        rep     stosw                       ; store AX full screen VRAM

        popf
        pop     es
    }
}
