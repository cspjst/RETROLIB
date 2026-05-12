#include "cga_lo_scroll.h"

void cga_lo_scroll_right(char* src, char* dst, cga_size_t width, cga_size_t height, cga_size_t stride) {
    __asm {
        .8086
        push    ds
        push    es
        pushf

        lds     si, src             ; DS:SI -> source bitmap
        les     di, dst             ; ES:DI -> destination bitamp
        mov     dx, height          ; DX = height
        mov     cx, width           ; CX = width
        cmp     cx, 16              ; minimum bitmap width
        jl      END

        shr     cx, 1               ; div 4 to get byte width
        shr     cx, 1               ; lo res mode 4 pixels per byte
        mov     bx, cx              ; copy byte width

YROW:   sub     cx, 2               ; zero index word
        add     si, cx              ; ES:SI -> src last word
        add     di, cx              ; DS:DI -> dst last word

        mov     ah, ds:[si]         ; load from src
        mov     al, ds:[si + 1]     ; little endian
        shr     ax, 1               ; shift along by 1 pixel
        shr     ax, 1               ; shift along by 1 pixel
        mov     es:[di], ah         ; store in dst
        mov     es:[di + 1], al     ; little endian

        jcxz    NEXT

 XCOL:  dec     si                  ; right to left
        dec     di                  ; right to left

        mov     ah, ds:[si]         ; load from src
        mov     al, ds:[si + 1]     ; ..
        shr     ax, 1               ; shift word along by 1 pixel
        shr     ax, 1
        mov     es:[di], ah         ; store in dst
        or      es:[di + 1], al     ; ..

        loop    XCOL                ; for all the bytes in the row

NEXT:   mov     ax, bx              ; calculate offset to next row
        add     ax, stride          ; width + stride
        add     si, ax              ; move src to start next src row
        add     di, ax              ; move dst to start next dst row

        mov     cx, bx              ; reset byte counter
        dec     dx                  ; next row
        jnz     YROW

END:    popf
        pop     es
        pop     ds
    }
}
