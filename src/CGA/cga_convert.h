/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.5.1
 */
#ifndef CGA_CONVERT_H
#define CGA_CONVERT_H

#include <stdint.h>

#include "../MEM/dos_mem_arena.h"
#include "cga_bitmap.h"

/**
 * Pre-shifts bitmap data for lo res mode 4 at 2 bits-per-pixel in order to
 * optimise block transfer to VRAM by providing pre-shifted bitmaps at
 * sub-byte x mod 4 positions 0..3 by trading RAM for performance.
 */
dos_memsize_t cga_convert_bmp_shifts_lo_res(cga_bitmap_t* bmp, mem_arena_t* arena);

/**
 * The PBM (P4) bitstream file format is a close match to the 1-bit-per-pixel (1bpp)
 * 8pixels per byte similarly LSB..MSB left..right pixel CGA mode 6 "hi res" layout.
 * However, and rather bizarrely, the PBM format pixel 1 is black and 0 is white.
 * @note Therefore, the image creator/artist is responsible for inverting the
 *       palette (1=white, 0=black) *before* saving as PBM raw format to ensure
 *       correct visual appearance in CGA mode 6.
 */
dos_memsize_t cga_convert_pbm_to_raw(
    const char* pbm_file_in_path,
    const char* pbm_file_out_path,
    mem_arena_t* arena
);

/**
 * Conversely, the PPM (P6) format stores 24-bit RGB triples per pixel which is not,
 * by any stretch of the imagination, a good match to the CGA Mode 4/5 low-res VRAM
 * layout of 2 bits per pixel (2bpp) 4 pixels per byte.
 * Conversion requires reading 3 RGB bytes per input pixel (96,000 bytes for 320×200)
 * and quantizing each RGB triplet to a 2-bit CGA palette index then packing 4 indices
 * into one output byte with correct bit ordering - this can be slow.
 * For example on a 4.77 MHz IBM XT (or faithful emulation), a full-screen 320×200
 * PPM to CGA 2bpp conversion takes ~19.6 seconds.
 * Ergo, slow convert once and fast load raw subsequently.
 */
dos_memsize_t cga_convert_ppm_to_raw(
    const char* ppm_file_in_path,
    const char* ppm_file_out_path,
    mem_arena_t* arena
);

#endif
