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
    //printf("%u, %u, %u, %u",x,y,w,h);
    __asm {
        .8086
        push    ds
        push    es
        pushf
        // 1.0 prepare registers
        mov     di, CGA_VIDEO_RAM_SEGMENT
        mov     es, di
        mov     di, dx
        shl     di, 1                       ; turn y into a word table index
        mov     di, CGA_ROW_OFFSETS[di]     ; ES:DI points to even VRAM
        lds     si, data                    ; DS:SI source RAM
        mov     dx, ax                      ; copy x
        cld                                 ; increase SI & DI
        // 1.1 calculate offset
        shr     dx, 1                       ; calculate column byte x / 8
        shr     dx, 1                       ; 8086 limited to single step shifts
        shr     dx, 1                       ;
        add     di, dx                      ; add in column byte
        // 1.2  convert pixel width to byte width
        xchg    bx, cx                      ; CX = width enable rep, BX = height
        shr     cx, 1                       ; calculate byte  width w / 8
        shr     cx, 1                       ; 8086 limited to single step shifts
        shr     cx, 1                       ;
        // 1.3 test if byte aligned x if so fast path REP MOVS
        test    ax, 7                       ; x modulo 8 is 0?
        jz      FAST                        ; no shifting needed
        // 2.0 TODO lodsb, shr, and mask or es[di], inc di
        jmp     END
FAST:   // 3.1 test if odd width skip MOVSB if even
        test    cx, 1
        jz      EVEN
        // 3.2 prepare next line step
        //mov     ax,
        mov     dx, cx                      ; copy byte width

        movsb                               ; AL = DS:SI
        dec     cx                          ; reduce count

        shr     cx, 1                       ; w/2 convert to word count
        rep     movsw

        mov     cx, dx                      ; restore CX


        jmp     END
EVEN:   // 4.2
        mov     dx, cx                      ; copy byte width

        shr     cx, 1                       ; w/2 convert to word count
        rep     movsw



END:    popf
        pop     es
        pop     ds
   */ }
}
