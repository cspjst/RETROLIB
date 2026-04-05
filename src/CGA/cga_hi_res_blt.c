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
    static const unsigned short ROW_OFFSETS[200] = {
       0x0000, 0x2000, 0x0050, 0x2050, 0x00A0, 0x20A0, 0x00F0, 0x20F0, 0x0140, 0x2140,
       0x0190, 0x2190, 0x01E0, 0x21E0, 0x0230, 0x2230, 0x0280, 0x2280, 0x02D0, 0x22D0,
       0x0320, 0x2320, 0x0370, 0x2370, 0x03C0, 0x23C0, 0x0410, 0x2410, 0x0460, 0x2460,
       0x04B0, 0x24B0, 0x0500, 0x2500, 0x0550, 0x2550, 0x05A0, 0x25A0, 0x05F0, 0x25F0,
       0x0640, 0x2640, 0x0690, 0x2690, 0x06E0, 0x26E0, 0x0730, 0x2730, 0x0780, 0x2780,
       0x07D0, 0x27D0, 0x0820, 0x2820, 0x0870, 0x2870, 0x08C0, 0x28C0, 0x0910, 0x2910,
       0x0960, 0x2960, 0x09B0, 0x29B0, 0x0A00, 0x2A00, 0x0A50, 0x2A50, 0x0AA0, 0x2AA0,
       0x0AF0, 0x2AF0, 0x0B40, 0x2B40, 0x0B90, 0x2B90, 0x0BE0, 0x2BE0, 0x0C30, 0x2C30,
       0x0C80, 0x2C80, 0x0CD0, 0x2CD0, 0x0D20, 0x2D20, 0x0D70, 0x2D70, 0x0DC0, 0x2DC0,
       0x0E10, 0x2E10, 0x0E60, 0x2E60, 0x0EB0, 0x2EB0, 0x0F00, 0x2F00, 0x0F50, 0x2F50,
       0x0FA0, 0x2FA0, 0x0FF0, 0x2FF0, 0x1040, 0x3040, 0x1090, 0x3090, 0x10E0, 0x30E0,
       0x1130, 0x3130, 0x1180, 0x3180, 0x11D0, 0x31D0, 0x1220, 0x3220, 0x1270, 0x3270,
       0x12C0, 0x32C0, 0x1310, 0x3310, 0x1360, 0x3360, 0x13B0, 0x33B0, 0x1400, 0x3400,
       0x1450, 0x3450, 0x14A0, 0x34A0, 0x14F0, 0x34F0, 0x1540, 0x3540, 0x1590, 0x3590,
       0x15E0, 0x35E0, 0x1630, 0x3630, 0x1680, 0x3680, 0x16D0, 0x36D0, 0x1720, 0x3720,
       0x1770, 0x3770, 0x17C0, 0x37C0, 0x1810, 0x3810, 0x1860, 0x3860, 0x18B0, 0x38B0,
       0x1900, 0x3900, 0x1950, 0x3950, 0x19A0, 0x39A0, 0x19F0, 0x39F0, 0x1A40, 0x3A40,
       0x1A90, 0x3A90, 0x1AE0, 0x3AE0, 0x1B30, 0x3B30, 0x1B80, 0x3B80, 0x1BD0, 0x3BD0,
       0x1C20, 0x3C20, 0x1C70, 0x3C70, 0x1CC0, 0x3CC0, 0x1D10, 0x3D10, 0x1D60, 0x3D60,
       0x1DB0, 0x3DB0, 0x1E00, 0x3E00, 0x1E50, 0x3E50, 0x1EA0, 0x3EA0, 0x1EF0, 0x3EF0
    };

    __asm {
        .8086
        push    ds
        push    es
        pushf

        // prepare registers
        mov     ax, x
        mov     bx, y
        shl     bx, 1                       ; turn y into a word table index
        mov     cx, w
        shr     cx, 1                       ; calculate byte  width w / 8
        shr     cx, 1                       ; 8086 limited to single step shifts
        shr     cx, 1                       ; CX is now byte width
        mov     dx, h
        //lds     si, data                    ; DS:SI -> data
        mov     di, CGA_VIDEO_RAM_SEGMENT
        mov     es, di
        cld                                 ; increase SI & DI
        // test if byte aligned x if so fast path REP MOVS
        test    ax, 7                       ; x modulo 8 is 0?
        jz      FAST                        ; no shifting needed
        // TODO lodsb, shr, and mask or es[di], inc di
        jmp     END
FAST:   test    cx, 1                       ; even or odd byte width
        jz      EVEN                        ; word transfers
        // calculate offset
        shr     ax, 1                       ; calculate column byte x / 8
        shr     ax, 1                       ; 8086 limited to single step shifts
        shr     ax, 1                       ;
        // prepare row word counter
        dec     cx                          ; reduce count to even
        shr     cx, 1                       ; w/2 convert to word count
        // loop for height
L3:     push cx                             ; copy byte width
        mov     di, ROW_OFFSETS[bx]         ; look-up row offset
        add     di, ax                      ; add in column
        stosb                               ; DS:SI->ES:DI
        //rep     movsw                       ; CX(DS:SI->ES:DI)
        pop     cx                          ; restore CX
        add     bx, 2                       ; next line look-up
        dec     dx
        //jnz     L3
        jmp     END
EVEN:   // 4.2
        shr     cx, 1                       ; w/2 convert to word count
        mov     dx, cx                      ; copy byte width


        rep     movsw



END:
        popf
        pop     es
        pop     ds
   }
}
