/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
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

// 8.25.3.3 Predefined "__watcall" calling convention [ax bx cx dx] stack
void __watcall cga_hi_res_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* data) {
    __asm {
        .8086
        push    ds
        push    es
        pushf

        lds     si, data                    ; DS:SI source RAM
        les     di, CGA_VRAM_PTR            ; ES:DI destination VRAM
        // 1.0 test if byte aligned x if so fast path REP MOVS
        test    ax, 7                       ; x modulo 8 is 0? 
        jz      FAST
        
        jmp     END
FAST:   // 2.1 test if odd width skip MOVSB if even
        test    ax, 1 
        jz      EVEN
        
        jmp     END
EVEN:   // 3.2 MOVSW width loop height
        
END:    popf
        pop     es
        pop     ds
    }
}
