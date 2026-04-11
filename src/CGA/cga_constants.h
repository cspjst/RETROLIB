/**
 * @author      Jeremy Simon Thornton
 * @copyright   2024 Jeremy Simon Thornton
 * @version     0.1.0
 */
#ifndef CGA_CONSTANTS_H
#define CGA_CONSTANTS_H

/**
 *  @def   CGA_VIDEO_RAM_SEGMENT
 *  @brief CGA VRAM starts at B8000 (736K bytes into the 8086 addressable 1MB)
 *  @details Both Low-res 320x200, 4-color (video modes 04H and 05H) and Hi-res 640x200, 2-color (video mode 06H)
 *  Layout : Interleaved scan lines, packed - pixel.
 *  Even - numbered scan lines begin at segment B800
 *  Odd - numbered scan lines begin at segment BA00
 */
#define CGA_VIDEO_RAM_SEGMENT	0B800h
#define CGA_EVEN_VRAM_SEGMENT	0B800h
#define CGA_ODD_VRAM_SEGMENT	0BA00h
#define CGA_BYTES_PER_BANK      2000h
#define CGA_BANK_BIT            2000h
#define CGA_BYTES_PER_ROW		80
#define CGA_WORDS_PER_ROW       40
#define CGA_ROWS_PER_SCREEN     200
#define CGA_ROWS_PER_BANK       100
#define CGA_SCREEN_BYTES        16384
#define CGA_SCREEN_WORDS        8192

// CGA Mode Control Register at I/O address
#define CGA_CONTROL_REG			3D8h
// CGA Color select palette register
#define CGA_PALETTE_REG         3D9h
// CGA Status Register at I/O address
#define CGA_STATUS_REG			3DAh

// When bit 0 of the CGA Mode Control Register is set the CPU can touch the CGA buffer without causing snow
#define CGA_CTRL_BIT0	1
// When bit 3 of the CGA Mode Control Register is set during vertical retrace.
#define CGA_CTRL_BIT3	8

#endif
