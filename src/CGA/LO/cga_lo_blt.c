#include "cga_lo_blt.h"

#include "../cga_constants.h"

#include <stdio.h>

void cga_lo_screen_blt(const char* data) {
    __asm {
        .8086
        push    ds
        push    es
        push    bp
        pushf

        cld                                 ; incremental MOVSW
        mov     di, CGA_VIDEO_RAM_SEGMENT   ; load even VRAM segment address
        mov     es, di                      ; ES:DI -> VRAM
        xor     di, di                      ; 0 is an even number so bank 0
        mov     ax, 1FB0h                   ; 2000h - 80 byte row increment & because add reg, reg is 1 cycle faster than xor reg,imm on 8086
        mov     bx, CGA_WORDS_PER_ROW       ; 40 words per row
        mov     dx, CGA_ROWS_PER_BANK       ; 100 rows/bank
        lds     si, data                    ; DS:SI -> linear source
        mov     bp, CGA_BYTES_PER_BANK      ; 2000h row decrement

ROWS:   mov     cx, bx                      ; load REP count
        rep     movsw                       ; copy 40 word row to bank 0
        add     di, ax                      ; start of bank 1
        mov     cx, bx                      ; load REP count
        rep     movsw                       ; copy 40 word row to bank 1
        sub     di, bp                      ; start of bank 0
        dec     dx                          ; next 2 rows
        jnz     ROWS                        ; until done

        popf
        pop     bp
        pop     es
        pop     ds
    }
}

void cga_lo_set_blt(cga_coord_t x, cga_coord_t y, cga_size_t width, cga_size_t height, const char** ptrs) {
    cga_size_t index, cseg, coff;
    index = cseg = coff = 0;
    __asm {
        .8086
        push    ds
        pushf

        mov     ax, CGA_VIDEO_RAM_SEGMENT
        mov     es, ax          ; ES = VRAM segment
        lds     si, ptrs        ; DS:SI = char**

        mov     ax, x           ; AX = x
        mov     di, ax          ; DI = x
        shr     di, 1           ; DI = x div 4
        shr     di, 1           ; DI is now VRAM byte column aligned
        and     ax, 3           ; AX = x mod 4
        mov     index, ax
        shl     ax, 1           ; Convert to sizeof char* index
        shl     ax, 1           ; AX = (x mod 4) * 4
        add     si, ax          ; offset to correct char*
        lds     si, ds:[si]     ; DS:SI -> pre shifted data
        mov     ax, ds
        mov     cseg, ax
        mov     coff, si


        popf
        pop     ds
    }
    printf("data[%hu] = %X%X\n", index, cseg, coff);
}
