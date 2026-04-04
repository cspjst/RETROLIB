/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#include "cga_hi_res_blt.h"
#include "cga_constants.h"
#include "cga_lookup_table_y.h"

void cga_hi_res_screen_blt(const char* data) {
    __asm {
        .8086
        push    ds
        push    es
        pushf

        lds     si, data                    ; DS:SI source RAM
        mov     ax, CGA_VIDEO_RAM_SEGMENT
        mov     es, ax
        mov     di, CGA_EVEN_BANK           ; ES:DI points to even VRAM
        cld                                 ; increment DI
        mov     cx, CGA_WORDS_PER_BANK      ; 200 even line buffer words
        rep     movsw                       ; copy to VRAM
        mov     di, CGA_ODD_BANK            ; ES:DI points to odd VRAM
        mov     cx, CGA_WORDS_PER_BANK      ; 200 odd line buffer words
        rep     movsw                       ; copy to VRAM

        popf
        pop     es
        pop     ds
    }
}

void __watcall cga_hi_res_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* data) {
    // AX = x, DX = y, BX = width, CX = height
    __asm {
        .8086
        push    ds
        push    es
        pushf
        // 1.0 prepare registers
        lds     si, data                    ; DS:SI source RAM
        mov     di, CGA_VIDEO_RAM_SEGMENT
        mov     es, di
        sub     di, di                      ; ES:DI points to even VRAM
        xchg    bx, cx                      ; CX = width, BX = height
        mov     di, dx                      ; DI = y
        shl     di, 1                       ; turn y into a word table index
        mov     di, CGA_ROW_OFFSETS[di]     ; load the VRAM row offset address
        mov     dx, ax                      ; copy x
        // 1.1 calculate offset
        shr     dx, 1                       ; calculate column byte x / 8
        shr     dx, 1                       ; 8086 limited to single step shifts
        shr     dx, 1                       ;
        add     di, dx                      ; add in column byte
        // 1.2 test if byte aligned x if so fast path REP MOVS
        test    ax, 7                       ; x modulo 8 is 0?
        //jz      FAST                        ; no shifting needed
/*

        jmp     END
FAST:   // 2.1 test if odd width skip MOVSB if even
        test    cx, 1
        jz      EVEN
        // 2.2 wallpaper the first byte

        dec     cx
        jz      END
EVEN:   // 3.2 MOVSW width loop height
        shr     cx, 1                       ; w/2 convert to word count
        jcxz    END                         ; width was zero

L1:     mov     ax, cx                      ; copy CX
        mov     bx, di                      ; copy DI
        rep     movsw                       ; move row of words
        mov     di, bx                      ; restore DI
        add     di, CGA_BYTES_PER_LINE      ; next row
        mov     cx, ax
        dec     dx
        jnz     L1

END:    pop     bp
        popf
        pop     es
        pop     ds
   */ }
}
