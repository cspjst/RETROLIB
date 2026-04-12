/**
* @author      Jeremy Simon Thornton
* @copyright   2022,2026 Jeremy Simon Thornton
* @version     0.1.1
* 2026 04 02 use __watcall for performance at the expense compiler agnostic
*/
#include "cga_hi_res_plot.h"
#include "../cga_constants.h"
#include "../cga_lookup_table_y.h"

/**
 * Use fast bit manipulation calculate row byte y/2 * 80 bytes per row
 * Use CL to rotate mask and shift pixel bit
 */
void cga_hi_res_plot_calculate(cga_coord_t x, cga_coord_t y, cga_hi_res_colours_t colour) {
    __asm {
        .8086

        mov     ax, CGA_EVEN_VRAM_SEGMENT   ; assume even lines segment
        mov     bx, y                       ; load y into bx
        test    bx, 1h                      ; is y even ?
        jz      EVEN                        ; yes skip
        mov     ax, CGA_ODD_VRAM_SEGMENT    ; otherwise load odd lines segment
EVEN:   mov     es, ax                      ; transer segment into es
        mov     dx, x                       ; load x into dx
        mov     cx, dx                      ; copy of x in cx
        and     cx, 7h                      ; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2)
        mov     al, 01111111b               ; load al with pixel mask
        ror		al, cl                      ; roll mask around by x mod 8
        // NB trying to optimise 0 colour slows white more than it speeds black
        mov     ah, colour                  ; load ah with a single pixel at msb (e.g. white 10000000)
        shr     ah, cl                      ; shift single bit along by x mod 8
        shr     dx, 1                       ; calculate column byte x / 8
        shr     dx, 1                       ; 8086 limited to single step shifts
        shr     dx, 1                       ;
        and     bx, 0FFFEh                  ; mask out even / odd row bit from y
        shl     bx, 1                       ; 8086 shift left 3 time
        shl     bx, 1                       ; as per binary simplification of y/2 * 80
        shl     bx, 1                       ;
        mov     cx, bx                      ; temp result in cx
        shl     cx, 1                       ; 8086 shift left twice
        shl     cx, 1                       ;
        add     bx, cx                      ; add back into bx
        add     bx, dx                      ; add in column byte
        and		es:[bx], al		            ; mask out the pixel bits
        or		es:[bx], ah		            ; plot point
    }
}

/**
 * 23.80% faster than cga_hi_res_plot_calculate
 * Use a lookup table for y is 16.7% faster
 * Use __fastcall for 6.12% faster AX = x, DX = y, BX = colour
 */
void __fastcall cga_hi_res_plot_lookup(cga_coord_t x, cga_coord_t y, cga_hi_res_colours_t colour) {
    // AX = x, DX = y, BX = colour
    __asm {
        .8086
        // 1. prepare registers
        mov     cx, CGA_VIDEO_RAM_SEGMENT   ; load the VRAM segment address
        mov     es, cx                      ; transer segment into es
        xchg    bx, dx                      ; BX = y DX = colour
        shl     bx, 1                       ; turn y into a word table index
        mov     bx, CGA_ROW_OFFSETS[bx]     ; load the VRAM row offset address
        mov     cx, ax                      ; copy of x in CX
        // 2. calculate colour and mask
        and     cx, 7h                      ; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2)
        mov     dh, 01111111b               ; load DH with pixel mask
        ror		dh, cl                      ; roll mask around by x mod 8
        shr     dl, cl                      ; shift single bit along by x mod 8
        // 3. calculate offset
        shr     ax, 1                       ; calculate column byte x / 8
        shr     ax, 1                       ; 8086 limited to single step shifts
        shr     ax, 1                       ;
        add     bx, ax                      ; add in column byte
        // 4. plot point
        and		es:[bx], dh		            ; mask out the pixel bits
        or		es:[bx], dl		            ; plot point
    }
}
