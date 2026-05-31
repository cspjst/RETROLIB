#include "cga_lo_blit.h"
#include "../cga_constants.h"

#include <stdio.h>

void cga_lo_paste_blit(cga_size_t x, cga_size_t y, cga_bitmap_t* bmp) {
    cga_size_t nblocks;
    _asm {
        .8086
        push    ds
        pushf

        mov     ax, CGA_VIDEO_RAM_SEGMENT
        mov     es, ax          ; ES = VRAM segment
        lds     si, bmp         ; DS:SI = bmp*

        mov     ax, x           ; AX = x
        mov     di, ax          ; DI = x copy
        shr     di, 1           ; DI = x div 4
        shr     di, 1           ; ES:DI = VRAM* byte column aligned

        mov     cx, ds:[si + BMP_BLOCKS]   ; number of bitmap data blocks
        mov     nblocks, cx
        jcxz    BLIT            ; 0 = only 1 bitmap data block
        and     ax, 3           ; AX = x mod 4 ie which shift data block
        mov     cx, 0FFFFh
BLIT:

        popf
        pop     ds
    }
    printf("nblocks = %i\n", nblocks);
    //printf("data[%hu] = %X%X\n", index, cseg, coff);
}
