#include "cga_lo_blit.h"
#include "../cga_constants.h"
#include "../cga_lookup_table_y.h"

#include <stdio.h>

void cga_lo_paste_blit_36(cga_size_t x, cga_size_t y, cga_bitmap_t* bmp) {
    cga_size_t noff, nseg;
    _asm {
        .8086
        push    ds
        pushf

        les     bx, bmp                     ; es:bx = bmp*

        mov     ax, x                       ; AX = x
        mov     di, ax                      ; DI = x copy

        mov     cx, es:[bx + BMP_BLOCKS]    ; number of bitmap data blocks
        jcxz    BLIT                        ; 0 = only 1 bitmap data block
        and     ax, 3                       ; AX = x mod 4 ie which shift data block
        mov     cx, ax

BLIT:   shl     cx, 1                       ; convert CX to double word offset
        shl     cx, 1                       ; 8086 single shifts only
        add     bx, cx                      ;
        mov     si, es:[bx + BMP_DATA + 2]  ; offset of data block
        mov     ax, es:[bx + BMP_DATA]      ; segment of data block
        mov     bx, y                       ; BX = y
        shl     bx, 1                       ; turn y into a word table index
        mov     bx, CGA_ROW_OFFSETS[bx]     ; BX = row offset address

        // test block addr select
        mov     ds, ax                      ; DS:SI = data*
        mov     nseg, ax
        mov     noff, si
        //lodsb
        //mov     num, al


        shr     di, 1                       ; DI = x div 4
        shr     di, 1                       ; DI = byte column
        add     di, bx                      ; DI = column + row

        //mov     ax, CGA_VIDEO_RAM_SEGMENT
        //mov     es, ax                      ; ES:DI = VRAM*

        //mov     ax, 0AAAAh
        //stosb


        popf
        pop     ds
    }
    printf("nseg = %X noff = %X\n", nseg, noff);
}
