#ifndef CGA_LO_SCROLL_H
#define CGA_LO_SCROLL_H

#include "../cga_types.h"

void cga_lo_scroll_right(char* src, char* dst, cga_size_t width, cga_size_t height, cga_size_t stride);

void cga_lo_scroll_left();

void cga_lo_scroll_down();

void cga_lo_scroll_up();

#endif
