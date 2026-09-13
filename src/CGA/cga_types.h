/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 *
 */
#ifndef CGA_TYPES_H
#define CGA_TYPES_H

typedef unsigned short cga_size_t;
typedef unsigned short cga_coord_t;
typedef unsigned short cga_colour_depth_t;
typedef unsigned char cga_colour_t;

typedef union {
    unsigned long p;        // packed: 0xXXXXYYYY
    struct {
        cga_coord_t x;
        cga_coord_t y;
    };
} cga_point_t;

typedef union {
    unsigned long argb;         // packed: 0x00RRGGBB
    struct {
        cga_colour_t blue;     // little endian
        cga_colour_t green;
        cga_colour_t red;
        cga_colour_t alpha;
    };
} cga_argb_t;

#endif
