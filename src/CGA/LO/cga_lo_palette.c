#include "cga_lo_palette.h"
#include "../cga_constants.h"
#include <stdint.h>
#include <stdio.h>

/**
 * CGA_PALETTE_REG (port 0x3D9) Bit reference:
 * Bit 5: Palette Select (0=Palette 0 G/R/Brown, 1=Palette 1 C/M/White)
 * Bit 4: Unused
 * Bit 3: Intensity (0=Low, 1=High)
 * Bits 2-0: Background/Border colour (RGB)
 * @note On real CGA hardware port 3D9h palette register is *write-only*
 * However, the BIOS itself maintains its own shadow of 0x3d8 and 0x3D9 in the BIOS Data Area:
 * 40:63	word	Base port address for active 6845 CRT controller
 *			3B4h = mono, 3D4h = color
 *	40:65	byte	6845 CRT mode control register value (port 3x8h)
 *			EGA/VGA values emulate those of the MDA/CGA
 *	40:66	byte	CGA current color palette mask setting (port 3d9h)
 *			EGA and VGA values emulate the CGA
 */
static const uint8_t PALETTE_TABLE[6] = {
    0x00, // Mode 4: Palette 0, Low Intensity  (bits: 0000 0000)
    0x08, // Mode 4: Palette 0, High Intensity (bits: 0000 1000)
    0x20, // Mode 4: Palette 1, Low Intensity  (bits: 0010 0000)
    0x28, // Mode 4: Palette 1, High Intensity (bits: 0010 1000)
    0x20, // Mode 5: Composite, Low Intensity  (bits: 0010 0000)
    0x28  // Mode 5: Composite, High Intensity (bits: 0010 1000)
};


void cga_lo_set_palette(cga_palette_number_t pal) {
    __asm {
        .8086
        push    es

        mov     bl, pal
        xor     bh, bh
        mov     ax, 40h                 ; BDA segment
        mov     es, ax                  ; preserve DS
        // handle mode control register (port 3D8h)
        mov     al, es:[65h]            ; read BDA mode control shadow
        cmp     bl, 4                   ; CGA_PALETTE_2_DARK
        jb      MODE4                   ; palette 0 or 1 - colour mode
        or      al, 04h                 ; set bit 2 B/W for mode 5
        jmp     MODE5
MODE4:  and     al, 0FBh                ; clear bit 2 for colour mode
MODE5:  mov     es:[65h], al            ; update BDA shadow
        mov     dx, CGA_CONTROL_REG     ; port 3D8h
        out     dx, al
        // handle palette register (port 3D9h)
        mov     al, es:[66h]            ; read BDA palette shadow
        and     al, 07h                 ; preserve background bits 2-0
        or      al, PALETTE_TABLE[bx]   ; merge palette and intensity bits
        mov     es:[66h], al            ; update BDA shadow
        mov     dx, CGA_PALETTE_REG     ; port 3D9h
        out     dx, al

        pop     es
    }
}

/**
 * Background/default colour is restricted to 3 bits
 * i.e. only
 *  BLACK       // 0000
 *  BLUE        // 0001
 *  GREEN       // 0010
 *  CYAN        // 0011
 *  RED         // 0100
 *  MAGENTA     // 0101
 *  BROWN       // 0110
 *  LTGRAY      // 0111 (IBM: "dark white")
 */
void cga_lo_set_background_colour(cga_palette_colour_t col) {
    __asm {
        .8086
        push    es

        mov     cl, col
        and     cl, 07h                 ; mask to 3 bits RGB
        mov     ax, 40h
        mov     es, ax
        mov     al, es:[66h]            ; read BDA palette shadow
        and     al, 0F8h                ; preserve bits 7..3
        or      al, cl                  ; merge background colour
        mov     es:[66h], al            ; update BDA shadow
        mov     dx, CGA_PALETTE_REG     ; port 3D9h
        out     dx, al

        pop     es
    }
}
