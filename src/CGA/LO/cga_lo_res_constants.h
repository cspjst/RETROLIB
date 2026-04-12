/**
 * @author      Jeremy Simon Thornton
 * @copyright   2024, 2026 Jeremy Simon Thornton
 * @version     0.1.1
 */
#ifndef CGA_LO_RES_CONSTANTS_H
#define CGA_LO_RES_CONSTANTS_H

 /**
  * Mode 4 macro definitions for assembly
  *
  * Graphic Mode: 320x200, 2-bit Color - this was the most popular CGA mode used for graphics.
  * The display resolution is 320x200 pixels with a pixel ratio of 1:1.2 (visually 320x240).
  * Only four colors could be displayed on the screen at a time, and they were mostly fixed, which is why so many CGA games look the same.
  * Mode 4 (0x04): The default for IBM PC compatibility.
  * Ensures the machine works correctly when hooked up to a household TV or composite monitor (common in the early 80s).
  * Mode 5 (0x05): Provided for users with dedicated RGB monitors (like the IBM 5153).
  * Provides a cleaner image and slightly more flexible palette control.
  *
  * Everything is hard coded to shave a few clock cycles off the execution time
  * Using #define enables immediate addressing mode - in which the data operand is a part of the instruction itself
  * This matters on an 8088 where reg,imm is only 4 clock cycles as opposed to 12+EA for reg,mem
  * This advantage dwindles on the 286 (2:5) and 386 (2:4), vanishing on the 486(1:1) and beyond
  */
#define CGA_LO_RES_SCREEN_X_MIN 0
#define CGA_LO_RES_SCREEN_X_MAX 320
#define CGA_LO_RES_SCREEN_Y_MIN 0
#define CGA_LO_RES_SCREEN_Y_MAX 200
#define CGA_LO_RES_SCREEN_Y_SCALE 0.625
#define CGA_LO_RES_PIXELS_PER_BYTE 4

#endif
