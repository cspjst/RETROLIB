#ifndef TEST_CGA_H
#define TEST_CGA_H

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "../CGA/cga_hi_res_constants.h"
#include "../CGA/cga_hi_res_plot.h"
#include "../CGA/cga_hi_res_cls.h"
#include "../CGA/cga_bitmap.h"
#include "../CGA/cga_hi_res_blt.h"

#include "../ENV/env_video_mode.h"
#include "../ENV/env_time.h"

#include "../MEM/dos_mem_arena.h"

#include "../LOG/log_tools.h"

#include "../../doslib/bioslib/src/BIOS/bios_clock_services.h"

#include <string.h>

void make_data_1bit(cga_bitmap_t* bmp) {
    if(!bmp || !bmp->data) return;
    uint16_t bpl = bmp->width / 8;           // bytes per line
    unsigned char* data = (unsigned char*)bmp->data;
    for (uint16_t y = 0; y < bmp->height; y++) {
        unsigned char* row = data + (y * bpl);
        if (y & 1) memset(row, 0x55, bpl);
        else memset(row, 0xFF, bpl);
    }
}

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

/* 4x4 Bayer Dithering Matrix for simulating grayscale */
static const uint8_t dither_matrix[4][4] = {
    {  0,  8,  2, 10 },
    { 12,  4, 14,  6 },
    {  3, 11,  1,  9 },
    { 15,  7, 13,  5 }
};

/* Helper: Draw a line using Bresenham's algorithm */
static void draw_line(cga_coord_t x0, cga_coord_t y0,
                      cga_coord_t x1, cga_coord_t y1,
                      cga_colour_t col) {
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

/* Helper: Draw a filled rectangle */
static void draw_rect(cga_coord_t x1, cga_coord_t y1,
                      cga_coord_t x2, cga_coord_t y2,
                      cga_colour_t col) {
    for (cga_coord_t y = y1; y <= y2; y++) {
        for (cga_coord_t x = x1; x <= x2; x++) {
            cga_plot(x, y, col);
        }
    }
}

/* Helper: Draw a circle using Midpoint Algorithm */
static void draw_circle(cga_coord_t cx, cga_coord_t cy,
                        cga_coord_t radius, cga_colour_t col) {
    int16_t x = radius;
    int16_t y = 0;
    int16_t err = 0;

    while (x >= y) {
        cga_plot(cx + x, cy + y, col); cga_plot(cx + y, cy + x, col);
        cga_plot(cx - y, cy + x, col); cga_plot(cx - x, cy + y, col);
        cga_plot(cx - x, cy - y, col); cga_plot(cx - y, cy - x, col);
        cga_plot(cx + y, cy - x, col); cga_plot(cx + x, cy - y, col);

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

/* Helper: Simulate Gray using Dithering */
static void draw_dithered_pixel(cga_coord_t x, cga_coord_t y, uint8_t intensity) {
    /* Intensity 0-15 maps to the threshold in the matrix */
    uint8_t threshold = dither_matrix[y % 4][x % 4];
    cga_plot(x, y, (intensity > threshold) ? CGA_WHITE : CGA_BLACK);
}

/* Main Test Pattern Routine */
void test_pattern(void) {
    const cga_coord_t WIDTH = 640;
    const cga_coord_t HEIGHT = 200;
    const cga_coord_t CX = WIDTH / 2;
    const cga_coord_t CY = HEIGHT / 2;
    bios_ticks_since_midnight_t t1, t2;

    bios_read_system_clock(&t1);
    cga_hi_res_cls(0xAA);

    /* 1. Paint White Background */
    draw_rect(0, 0, WIDTH - 1, HEIGHT - 1, CGA_WHITE);

    /* Horizontal center line */
    draw_line(0, CY, WIDTH - 1, CY, CGA_BLACK);

    /* Vertical center line */
    draw_line(CX, 0, CX, HEIGHT - 1, CGA_BLACK);

    /* Diagonal top-left to bottom-right */
    draw_line(0, 0, WIDTH - 1, HEIGHT - 1, CGA_BLACK);

    /* 2. Central Resolution Wedges (Simulated with concentric circles) */
    for (uint8_t r = 10; r < 90; r += 5) {
        draw_circle(CX, CY, r, CGA_BLACK);
    }

    /* 3. Top Grayscale Bar (Dithered) */
    for (cga_coord_t x = 50; x < 590; x++) {
        /* Map X position to intensity 0-15 */
        uint8_t intensity = (uint8_t)((x - 50) * 15 / 540);
        for (cga_coord_t y = 10; y < 30; y++) {
            draw_dithered_pixel(x, y, intensity);
        }
    }

    /* 4. Geometric Calibration Shapes (Bottom) */
    draw_rect(100, 160, 140, 190, CGA_BLACK); /* Square */

    /* Triangle (Simple line approximation) */
    for (cga_coord_t i = 0; i < 30; i++) {
        for (cga_coord_t x = 200 - i; x <= 200 + i; x++) {
            cga_plot(x, 175 + i, CGA_BLACK);
        }
    }

    /* 5. Crosshair */
    for (cga_coord_t i = 0; i < 20; i++) {
        cga_plot(CX + i, CY, CGA_BLACK);
        cga_plot(CX - i, CY, CGA_BLACK);
        cga_plot(CX, CY + i, CGA_BLACK);
        cga_plot(CX, CY - i, CGA_BLACK);
    }

    bios_read_system_clock(&t2);
    printf("Time = %fsec\n", env_ticks_to_seconds(t2-t1));
}

void test_screen_blt() {
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");

    char fname[] = "../res/joker.pbm";
    cga_bitmap_t bmp = {0};
    FILE* f = fopen(fname, "rb");
    if(!f) printf("fopen: %s \"%s\"\n", strerror(errno), fname);

    assert(cga_read_meta_raw_pbm(f, &bmp));
    if(!f) printf("%s \"%s\"\n", strerror(errno), fname);
    //printf("%hu x %hu\n", bmp.width, bmp.height);
    bmp.data = (char*)mem_arena_alloc(arena, bmp.size);
    if(!bmp.data) printf("Failed to allocate %lu bytes!\n", bmp.size);
    assert(cga_load_bmp_raw_pbm(f, &bmp) == bmp.size);

    //cga_hi_res_cls(0xFF);
    //make_data_1bit(&bmp);
    cga_hi_res_screen_blt(bmp.data);

    fclose(f);
    mem_free_arena(arena);
}

void test_blt() {
    int w = 32;
    int h = 32;
    mem_arena_t* arena = mem_new_arena(4096);   // 64K
    if(!arena) printf("Failed to create arena!\n");
    cga_bitmap_t bmp;
    cga_make_bmp(&bmp, 1, w, h);
    bmp.data = (char*)mem_arena_alloc(arena, bmp.size);
    make_data_1bit(&bmp);

    cga_plot(318, 0, CGA_WHITE);
    cga_hi_res_blt(320, 1, bmp.width, bmp.height, bmp.data);

    mem_free_arena(arena);
}

void test_cga() {
    bios_video_mode_t m = env_get_video_mode();
    env_set_video_mode(CGA_GRAPHICS_MONOCHROME_640X200);

    //test_pattern();
    //test_screen_blt();
    test_blt();

    getchar();
    env_set_video_mode(m);
}

#endif
