/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 */
#include "fxp_int0_handler.h"
#include <stdio.h>

#include "../../doslib/src/DOS/dos_services.h"
#include "../../doslib/src/DOS/dos_services_constants.h"
#include "../../doslib/src/DOS/dos_error_codes.h"

static void* old_int0_handler;

static const char INT0_MESSAGE[] = "Your program caused an fxp_div overflow error...$";

// Declares a variable dos_int0 that can hold the address of an interrupt handler.
static void (__interrupt __far* dos_int0)(void);

/**
 * In Watcom C, __interrupt declares a function as an interrupt handler.
 * The compiler generates an IRET return rather than an ordinary RET,
 * and saves/restores the registers required by the interrupt calling convention.
 */
void __interrupt __far fxp_int0_handler() {
    dos_set_interrupt_vector(0, dos_int0);  // restore DOS INT0 handler
    __asm {
        .8086
        lea     dx, INT0_MESSAGE
        mov     ah, DOS_PRINT_STRING
        int     DOS_SERVICE

        mov     al, DOS_INVALID_DATA
        mov     ah, DOS_TERMINATE_PROCESS_WITH_RETURN_CODE
        int     DOS_SERVICE
    }
}

void fxp_install_int0_handler() {
    dos_int0 = dos_get_interrupt_vector(0);
    dos_set_interrupt_vector(0, (void __far*)fxp_int0_handler);
}

void fxp_uninstall_int0_handler() {
    dos_set_interrupt_vector(0, dos_int0);
}
