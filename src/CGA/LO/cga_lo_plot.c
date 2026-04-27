/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#include "cga_lo_plot.h"
#include "../cga_constants.h"
#include "../cga_lookup_table_y.h"  // IWYU pragma: keep

void __fastcall cga_lo_res_plot(cga_coord_t x, cga_coord_t y, cga_lo_res_colour_t colour) {
    // AX = x, DX = y, BX = colour
    __asm {
        .8086
        mov     cx, CGA_VIDEO_RAM_SEGMENT   ; load the VRAM segment address
        mov     es, cx                      ; transer segment into es
        xchg    bx, dx                      ; BX = y DX = colour
        shl     bx, 1                       ; turn y into a word table index
        mov     bx, CGA_ROW_OFFSETS[bx]     ; load the VRAM row offset address
        mov     cx, ax                      ; copy of x in CX
        and     cx, 3h                      ; mask off 0011 lower bits i.e.mod 4 pixels per byte
        shl     cx, 1                       ; *2 as 2 colour bits per pixel
        mov     dh, 00111111b               ; load DH with pixel mask
        ror     dh, cl                      ; roll mask right (msb->lsb) x mod 4
        shr     dl, cl                      ; shift colour right similarly
        shr     ax, 1                       ; calculate column byte x / 4
        shr     ax, 1                       ; 8086 limited to single step shifts                       ;
        add     bx, ax                      ; add in column byte
        and		es:[bx], dh		            ; mask out the pixel bits
        or		es:[bx], dl		            ; or in the pixel colour
    }
}
