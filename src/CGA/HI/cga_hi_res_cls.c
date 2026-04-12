/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#include "cga_hi_res_cls.h"
#include "cga_hi_res_colours.h"
#include "../cga_constants.h"

void __fastcall cga_hi_res_cls(cga_hi_res_colours_t pattern) {
    __asm {
        .8086
        push    es
        pushf

        cld                                 ; incremental STOSW
        mov     di, CGA_VIDEO_RAM_SEGMENT
        mov     es, di
        xor     di, di
        mov     cx, CGA_SCREEN_WORDS        ; 2000h words
        mov     ah, al                      ; duplicate pattern byte
        rep     stosw                       ; store AX full screen VRAM

        popf
        pop     es
    }
}
