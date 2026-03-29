/*
IBM 5140 PC Convertible detection via BIOS equipment word
#define IBM_5140_CONVERTIBLE() \
    (((*(volatile unsigned short far*)0x0040:0x0010) & 0x3000) == 0x3000)

Optimized constants for 5140 native Mode 6
#if defined(TARGET_IBM_5140)
    #define CGA_MODE_SELECT 0x06
    #define CGA_SCREEN_WIDTH 640
    #define CGA_SCREEN_HEIGHT 200
    #define CGA_PIXEL_ASPECT_RATIO 1.0
    #define CGA_PIXELS_PER_BYTE 8
    #define CGA_VIDEO_SEGMENT 0xB800
LCD requires no color burst, no palette tricks
    #define CGA_COLOR_SELECT_INIT 0x00
#endif
 */
