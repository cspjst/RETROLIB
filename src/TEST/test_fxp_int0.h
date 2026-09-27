#ifndef TEST_FXP_INT0_H
#define TEST_FXP_INT0_H

#include <stdio.h>
#include "../FXP/fxp_types.h"
#include "../FXP/fxp_conversions.h"
#include "../FXP/fxp_operators.h"
#include "../FXP/fxp_trigonometry.h"


static void test_int0(void) {
    int choice;

    printf("Trigger which panic?\n"
           "  0 = DIV  (divide fault)\n"
           "  1 = SQRT (negative domain)\n"
           "  2 = EXP  (argument out of range)\n"
           "> ");
    scanf("%d", &choice);

    switch (choice) {
        case 0: {
            fxp16_t zero = 0;
            fxp_div(FXP_ONE, zero);                     /* hardware IDIV fault */
            break;
        }
        case 1:
            fxp_sqrt(fxp_fix_float(-1.0f));                    /* deliberate domain panic */
            break;
        case 2:
            fxp_exp((fxp16_t)(FXP_EXP_NEG_BOUND - 1));   /* deliberate domain panic */
            break;
        default:
            printf("no panic - unrecognised choice %d\n", choice);
            break;
    }
}

#endif
