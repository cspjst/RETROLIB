#ifndef TEST_CGA_H
#define TEST_CGA_H

#include <stdio.h>
#include "../CGA/cga_detect_adapter.h"

void test_cga() {
    printf("%s\n", cga_detect_adapter() ?"CGA card" :"NO CGA crad!");
}

#endif
