#ifndef TEST_LO_CGA_H
#define TEST_LO_CGA_H

#include <stdint.h>
#include <string.h>

#include "../CGA/LO/cga_lo_res_plot.h"
#include "../CGA/LO/cga_lo_res_colours.h"

#include "../CGA/cga_bitmap.h"

#include "../ENV/env_video_mode.h"
#include "../ENV/env_time.h"

/*
void make_data_2bit(cga_bitmap_t* bmp) {
    if(!bmp || !bmp->data) return;
    uint16_t bpl = bmp->width / 4;           // bytes per line (2 bits/pixel)
    unsigned char* data = (unsigned char*)bmp->data;
    for (uint16_t y = 0; y < bmp->height; y++) {
        unsigned char* row = data + (y * bpl);
        switch (y % 4) {
            case 0: memset(row, 0x00, bpl); break;  // Black
            case 1: memset(row, 0x55, bpl); break;  // Color 1
            case 2: memset(row, 0xAA, bpl); break;  // Color 2
            case 3: memset(row, 0xFF, bpl); break;  // Color 3
        }
    }
}

// 4x4 Bayer Dithering Matrix for simulating more colors
static const uint8_t dither_matrix[4][4] = {
    {  0,  8,  2, 10 },
    { 12,  4, 14,  6 },
    {  3, 11,  1,  9 },
    { 15,  7, 13,  5 }
};

// Helper: Draw a line using Bresenham's algorithm
static void draw_line_low(cga_coord_t x0, cga_coord_t y0,
                          cga_coord_t x1, cga_coord_t y1,
                          cga_lo_res_colours_t col) {
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    int16_t sx = (dx < 0) ? -1 : 1;
    int16_t sy = (dy < 0) ? -1 : 1;
    int16_t err = ((dx > dy) ? dx : -dy) / 2;
    int16_t e2;

    dx = (dx < 0) ? -dx : dx;
    dy = (dy < 0) ? -dy : dy;

    for (;;) {
        cga_plot(x0, y0, col);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

// Helper: Draw a filled rectangle
static void draw_rect_low(cga_coord_t x1, cga_coord_t y1,
                          cga_coord_t x2, cga_coord_t y2,
                          cga_lo_res_colours_t col) {
    for (cga_coord_t y = y1; y <= y2; y++) {
        for (cga_coord_t x = x1; x <= x2; x++) {
            cga_plot_low(x, y, col);
        }
    }
}

// Helper: Draw a circle using Midpoint Algorithm
static void draw_circle_low(cga_coord_t cx, cga_coord_t cy,
                            cga_coord_t radius, cga_low_res_colours_t col) {
    int16_t x = radius;
    int16_t y = 0;
    int16_t err = 0;

    while (x >= y) {
        cga_plot(cx + x, cy + y, col);
        cga_plot(cx + y, cy + x, col);
        cga_plot(cx - y, cy + x, col);
        cga_plot(cx - x, cy + y, col);
        cga_plot(cx - x, cy - y, col);
        cga_plot(cx - y, cy - x, col);
        cga_plot(cx + y, cy - x, col);
        cga_plot(cx + x, cy - y, col);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

// Helper: Dither between two CGA colors
static void draw_dithered_pixel_low(cga_coord_t x, cga_coord_t y,
                                     uint8_t intensity,
                                     cga_low_res_colours_t col1,
                                     cga_low_res_colours_t col2) {
    uint8_t threshold = dither_matrix[y % 4][x % 4];
    cga_plot(x, y, (intensity > threshold) ? col1 : col2);
}

// Main Test Pattern Routine for CGA Mode 4
void test_lo_pattern(void) {
    const cga_coord_t WIDTH = 320;
    const cga_coord_t HEIGHT = 200;
    const cga_coord_t CX = WIDTH / 2;
    const cga_coord_t CY = HEIGHT / 2;
    bios_ticks_since_midnight_t t1, t2;

    bios_read_system_clock(&t1);

    // Clear screen with cyan (color 1)
    cga_low_res_cls(CGA4_CYAN);

    // 1. White border
    draw_rect_low(1, 1, WIDTH - 2, HEIGHT - 2, CGA4_WHITE);
    draw_rect_low(2, 2, WIDTH - 3, HEIGHT - 3, CGA4_BLACK);

    // 2. Center crosshair (magenta)
    draw_line_low(0, CY, WIDTH - 1, CY, CGA4_MAGENTA);
    draw_line_low(CX, 0, CX, HEIGHT - 1, CGA4_MAGENTA);

    // 3. Diagonal lines (white)
    draw_line_low(0, 0, WIDTH - 1, HEIGHT - 1, CGA4_WHITE);
    draw_line_low(WIDTH - 1, 0, 0, HEIGHT - 1, CGA4_WHITE);

    // 4. Concentric circles (magenta and cyan)
    for (uint8_t r = 10; r < 70; r += 10) {
        draw_circle_low(CX, CY, r, (r % 20 == 0) ? CGA4_MAGENTA : CGA4_CYAN);
    }

    // 5. Color bars (top) - cyan to magenta gradient via dithering
    for (cga_coord_t x = 20; x < 300; x++) {
        uint8_t intensity = (uint8_t)((x - 20) * 15 / 280);
        for (cga_coord_t y = 5; y < 20; y++) {
            draw_dithered_pixel_low(x, y, intensity, CGA4_CYAN, CGA4_MAGENTA);
        }
    }

    // 6. Geometric shapes (bottom left) - solid white
    draw_rect_low(10, 160, 50, 190, CGA4_WHITE);           // Square
    draw_circle_low(30, 175, 10, CGA4_MAGENTA);            // Circle inside

    // Triangle (filled using line approximation)
    for (cga_coord_t i = 0; i < 25; i++) {
        for (cga_coord_t x = 70 - i; x <= 70 + i; x++) {
            cga_plot_low(x, 175 + i, CGA4_CYAN);
        }
    }

    // 7. Text pattern (bottom right)
    const char* text = "CGA MODE 4";
    for (uint8_t i = 0; text[i]; i++) {
        for (uint8_t y = 0; y < 8; y++) {
            for (uint8_t x = 0; x < 8; x++) {
                // Simple 8x8 font simulation - just a block
                if (i * 8 + x < WIDTH - 20) {
                    cga_plot_low(220 + i * 8 + x, 180 + y, CGA4_WHITE);
                }
            }
        }
    }

    // 8. Checkerboard pattern (center area)
    for (cga_coord_t y = 80; y < 120; y++) {
        for (cga_coord_t x = 120; x < 200; x++) {
            cga_plot_low(x, y, ((x + y) % 16 < 8) ? CGA4_BLACK : CGA4_WHITE);
        }
    }

    bios_read_system_clock(&t2);
    printf("Mode 4 Test Pattern Time = %fsec\n", env_ticks_to_seconds(t2 - t1));
}
*/
void test_lo_cga() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_4_COLOUR_320X200);

    cga_plot(160, 99, CGA_CYAN);
    cga_plot(159, 98, CGA_MAGENTA);
    cga_plot(161, 100, CGA_WHITE);

    getchar();
    env_set_video_mode(m);
}

#endif
