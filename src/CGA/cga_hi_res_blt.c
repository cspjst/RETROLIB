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

void cga_hi_res_blt(cga_coord_t x, cga_coord_t y, cga_coord_t w, cga_coord_t h, const char* data) {
    __asm {
        .8086
        push    ds
        push    es
        push    bp
        pushf

        // prepare registers
        cld
        mov     ax, x                       ; AX = x
        shr     ax, 1                       ; calculate column byte x / 8
        shr     ax, 1                       ; 8086 limited to single step shifts
        shr     ax, 1                       ; AX is now *column* byte
        mov     bx, y                       ; BX = y
        mov     cx, w                       ; CX = width
        shr     cx, 1                       ; calculate byte  width w / 8
        shr     cx, 1                       ; 8086 limited to single step shifts
        shr     cx, 1                       ; CX is now *byte* width
        mov     dx, h                       ; DX = height
        mov     di, CGA_VIDEO_RAM_SEGMENT   ; more 8086 quirks
        mov     es, di                      ; ES = VRAM segment
        mov     di, bx                      ; DI = y
        shl     di, 1                       ; DI is a word offset
        mov     di, CGA_ROW_OFFSETS[di]     ; ES:DI -> VRAM
        add     di, ax                      ; ES:DI -> VRAM (x,y)
        // at this point BX is free to reuse
        mov     ax, CGA_BYTES_PER_LINE      ; 80 bytes per line CGA
        sub     ax, cx                      ; 80 - *byte* width
        lds     si, data                    ; DS:SI -> data (safe now)
        // test  x, 1 ; ?byte aligned x position
        // jz FAST

FAST:   mov     bx, cx                      ; copy CX byte width
        mov     ax, 2000h                   ; bank 1 add_stride
        sub     ax, cx                      ; add_stride - byte width
        mov     bp, 1FB0h                   ; bank 0 sub_stride - 80 next line
        add     bp, cx                      ; sub_stride + byte_width
        test    cx, 1                       ; even or odd byte width
        jz      EVEN                        ; word transfers
        test    di, 2000h
        jnz     OBANK1
OBANK0: mov     cx, bx                      ; set CX byte counter
        rep     movsb                       ; CX(DS:SI -> ES:DI)bytes
        mov     cx, bx                      ; restore CX
        dec     dx                          ; reduce row count
        jz      END                         ; no more rows
        add     di, ax                      ; bank1 - byte width

OBANK1: mov     cx, bx                      ; set CX byte counter
        rep     movsb                       ; CX(DS:SI -> ES:DI)bytes
        mov     cx, bx                      ; restore CX
        dec     dx                          ; reduce row count
        jz      END                         ; no more rows
        sub     di, bp                      ; bank 0

        jmp     OBANK0

EVEN:   shr     bx, 1
        test    di, 2000h
        jnz     EBANK1
EBANK0: mov     cx, bx                      ; set CX byte counter
        rep     movsw                       ; CX(DS:SI -> ES:DI)bytes
        dec     dx                          ; reduce row count
        jz      END                         ; no more rows
        add     di, ax                      ; bank1 - byte width

EBANK1: mov     cx, bx                      ; set CX byte counter
        rep     movsw                       ; CX(DS:SI -> ES:DI)bytes
        dec     dx                          ; reduce row count
        jz      END                         ; no more rows
        sub     di, bp                      ; bank 0

        jmp     EBANK0
END:
        popf
        pop     bp
        pop     es
        pop     ds
   }
}
