/**
 * @author      Jeremy Simon Thornton
 * @copyright   2024, 2026 Jeremy Simon Thornton
 * @version     0.2.5
 */
#ifndef CGA_HI_RES_COLOURS_H
#define CGA_HI_RES_COLOURS_H

/**
 * White is deliberating set MSB (rather than LSB) to enable fast plot routine shift right by low nybble
 */
typedef enum {
    CGA_BLACK,
    CGA_WHITE = 0x80    // 1000 0000
} cga_hi_res_colours_t;

#endif
