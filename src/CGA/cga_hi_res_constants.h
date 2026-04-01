/**
 * @author Jeremy Simon Thornton
 * @copyright 2024, 2026 Jeremy Simon Thornton
 * @version 0.1.1
 */
#ifndef CGA_HI_RES_CONSTANTS_H
#define CGA_HI_RES_CONSTANTS_H

#define CGA_WHITE   0x80
#define CGA_BLACK   0

/**
 * Mode 6 macro definitions for assembly
 *
 * Mode 6 (0x06): High resolution monochrome mode.
 * Graphic Mode: 640x200, 1-bit Color - this was the highest resolution CGA mode.
 * Typically used for sharp graphics or text-heavy interfaces where color is secondary to detail.
 * Unlike Mode 4/5, the palette register selects the foreground color only; background is fixed black.
 * The display resolution is 640x200 pixels. On standard CRTs, pixels are tall (1:1.2 PAR),
 * causing circles to appear as ovals unless corrected in software.
 *
 * Hardware Note: IBM PC Convertible (Model 5140)
 * Released April 1986, the Model 5140 features a native 640x200 supertwist LCD panel.
 * On this specific hardware, Mode 6 is the native framebuffer format:
 * Physical Pixel Grid: 640x200 square pixels (1:1 PAR).
 * Mapping: Direct 1:1 correspondence between video memory bits and LCD elements.
 * Result: Geometric fidelity is perfect without software aspect correction.
 * Color: Monochrome only (1bpp), matching the LCD's physical limitations.
 *
 * Everything is hard coded to shave a few clock cycles off the execution time
 * Using #define enables immediate addressing mode - in which the data operand is a part of the instruction itself
 * This matters on an 8088 where reg,imm is only 4 clock cycles as opposed to 12+EA for reg,mem
 * This advantage dwindles on the 286 (2:5) and 386 (2:4), vanishing on the 486(1:1) and beyond
 */
#define CGA_HI_RES_SCREEN_X_MIN 0
#define CGA_HI_RES_SCREEN_X_MAX 640
#define CGA_HI_RES_SCREEN_Y_MIN 0
#define CGA_HI_RES_SCREEN_Y_MAX 200
#define CGA_HI_RES_SCREEN_Y_SCALE 0.3125
#define CGA_HI_RES_PIXELS_PER_BYTE 8
#endif
