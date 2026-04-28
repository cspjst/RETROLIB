#ifndef TEST_SCROLL_H
#define TEST_SCROLL_H

#include <stdio.h>

void test_dump_data(int h, int w, const char* src) {
    for(int y = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            printf("%08b ", src[y * w + x]);
        }
        putchar('\n');
    }
}

void test_right() {
    char src[4][4] = {  {0xF1, 0x55, 0x55, 0x0F},
                        {0xF2, 0x55, 0x55, 0x0F},
                        {0xF3, 0x55, 0x55, 0x0F},
                        {0xF4, 0x55, 0x55, 0x0F} };
    char dst[4][4] = {0};
    test_dump_data(4, 4, &src[0][0]);
}

void test_scroll() {
    test_right();
}

#endif
