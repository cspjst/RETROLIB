#include "cga_lo_blit.h"

//#include <stdio.h>

#include "../cga_constants.h"
#include "../cga_lookup_table_y.h"

void cga_lo_paste_blit_36(cga_size_t x, cga_size_t y, cga_bitmap_t* bmp) {
    cga_size_t noff, nseg;
    noff = nseg = 0;
    _asm {
        .8086
        push    ds
        pushf

        cld                                 ; inc mode
        // setup ES:BX to point to the bitmap struct
        les     bx, bmp                     ; ES:BX = bmp*
        mov     ax, x                       ; AX = x
        mov     di, ax                      ; DI = x
        // select bitmap data block
        and     ax, 3                       ; AX = x mod 4 ie which 0..3 bitmap data block
        shl     ax, 1                       ; convert AX to double word offset
        shl     ax, 1                       ; 8086 single shifts only, MUL 4
        // setup width, height and DS:SI ptr to the correct bitmap data
        mov     cx, es:[bx + BMP_WIDTH]     ; CX = bitmap pixel width
        shr     cx, 1                       ; DIV 4 bits per byte
        shr     cx, 1                       ; CX = bitmap byte width
        mov     dx, es:[bx + BMP_HEIGHT]    ; DX = bitmap pixel height
        add     bx, ax                      ; select bitmap address
        mov     si, es:[bx + BMP_DATA]      ; SI = offset of data block
        mov     ax, es:[bx + BMP_DATA + 2]  ; AX = segment of data block

        // setup x (DI as above), y and ES:DI ptr to VRAM
        shr     di, 1                       ; DI = x div 4
        shr     di, 1                       ; DI = VRAM byte column
        mov     bx, y                       ; BX = y
        shl     bx, 1                       ; turn y into a word table index
        add     di, CGA_ROW_OFFSETS[bx]     ; DI = byte column + row offset
        mov     bx, CGA_VIDEO_RAM_SEGMENT
        mov     es, bx                      ; ES:DI = VRAM*
        mov     ds, ax                      ; DS:SI = data*

        test    cx, 1                       ; odd byte width?
        je      EVEN
        movsb                               ; paste 1 byte
        dec     cx                          ; width - 1
EVEN:   shr     cx, 1                       ; CX DIV 2 = word count
        rep movsw                           ; paste remainder


        popf
        pop     ds
    }
    //printf("nseg = %X noff = %X\n", nseg, noff);
}
