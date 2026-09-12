#ifndef TEST_FXP_INT0_H
#define TEST_FXP_INT0_H

#include <stdio.h>
#include "../FXP/fxp_int0_handler.h"
#include "../../doslib/src/DOS/dos_services.h"

static void test_fxp_int0() {
    printf("DOS INT 0 = %p\n", dos_get_interrupt_vector(0));
    fxp_install_int0_handler();
    printf("FXP INT 0 = %p\n", dos_get_interrupt_vector(0));
    int x = 1;
    x /= 0;
    printf("this line is not reached");
}

#endif
