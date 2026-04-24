#ifndef HGA_COLOURS_H
#define HGA_COLOURS_H

typedef enum {
    HGA_BLACK = 0,
    HGA_WHITE = 0x80     // 1000 0000b performance optimisation for easier shr
} hga_colour_t;

#endif
