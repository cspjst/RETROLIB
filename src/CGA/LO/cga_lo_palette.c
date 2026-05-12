#include "cga_lo_palette.h"
#include "../cga_constants.h"
#include <stdint.h>
#include <stdio.h>

static const uint8_t PALETTE_TABLE[6] = {
    0x00, // Mode 4: Palette 0, Low Intensity  (bits: 0000 0000)
    0x10, // Mode 4: Palette 0, High Intensity (bits: 0001 0000)
    0x20, // Mode 4: Palette 1, Low Intensity  (bits: 0010 0000)
    0x30, // Mode 4: Palette 1, High Intensity (bits: 0011 0000)
    0x20, // Mode 5: Composite, Low Intensity  (bits: 0010 0000)
    0x30  // Mode 5: Composite, High Intensity (bits: 0011 0000)
};

/**
 * CGA_PALETTE_REG (port 0x3D9) Bit reference:
 * Bit 5: Palette Select (0=Palette 0 R/G/Y, 1=Palette 1 M/C/W)
 * Bit 4: Intensity (0=Low, 1=High)
 * Bits 3-0: Background/Border colour (IRGB)
 * @note Colour burst is controlled by bit 2 of the mode control register at 0x3D8
 * To select unofficial Mode 5 palette disable ColorBurst.
 */
void cga_lo_set_palette(cga_palette_number_t pal) {
    if(pal > CGA_PALETTE_1_HI) {        // disable ColorBurst
        __asm {

        }
    }
    __asm {
        .8086
        mov     bl, pal
        xor     bh, bh
        mov     ax, 0x40
        mov     es, ax
        mov     al, es:[0x65]           ; read BDA shadow
        and     al, 0x07                ; preserve background bits
        or      al, PALETTE_TABLE[bx]   ; merge palette bits
        mov     es:[0x65], al           ; update BDA shadow
        mov     dx, CGA_PALETTE_REG
        out     dx, al
    }
}

void cga_lo_set_background_colour(cga_palette_colour_t col) {
    __asm {
        .8086
        mov     cl, col
        and     cl, 0x0F                ; mask to 4 bits
        mov     ax, 0x40
        mov     es, ax
        mov     al, es:[0x65]           ; read BDA shadow
        and     al, 0xF0                ; preserve bits 7..4
        or      al, cl                  ; merge background colour
        mov     es:[0x65], al           ; update BDA shadow
        mov     dx, CGA_PALETTE_REG
        out     dx, al
    }
}
