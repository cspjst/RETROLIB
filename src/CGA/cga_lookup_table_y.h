/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 *
 */
#ifndef CGA_LOOKUP_TABLE_Y_H
#define CGA_LOOKUP_TABLE_Y_H

#include "cga_constants.h"

/**
 * CGA Mode 6 Row Offset Table (600x200)
 * 200 entries, 2 bytes each = 400 bytes total
 * Stride = 80 bytes
 */
extern const unsigned short CGA_ROW_OFFSETS[CGA_ROWS_PER_SCREEN];

#endif
