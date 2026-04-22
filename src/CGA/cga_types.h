/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.0
 */
#ifndef CGA_TYPES_H
#define CGA_TYPES_H

typedef unsigned short cga_size_t;
typedef unsigned short cga_coord_t;
typedef unsigned short cga_colour_depth_t;

typedef unsigned char cga_colour_t;

typedef union {
    unsigned long argb;         // packed: 0x00RRGGBB
    struct {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
        unsigned char alpha;
    };
} cga_argb_t;

#endif
