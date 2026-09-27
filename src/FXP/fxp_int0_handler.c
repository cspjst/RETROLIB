/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 */
#include "fxp_int0_handler.h"
#include <stdio.h>

#include "../../doslib/src/DOS/dos_services.h"
#include "../../doslib/src/DOS/dos_services_constants.h"
#include "../../doslib/src/DOS/dos_error_codes.h"

#define FXP_PANIC_SLOT_SIZE   32

const char FXP_PANIC_MESSAGES[4][FXP_PANIC_SLOT_SIZE] = {
    "DIV: divide or overflow fault  $",
    "SQRT: negative domain          $",
    "EXP: argument out of range     $",
    "FIX: argument out of range     $",
};

static void* dos_int0_handler;

// Declares a variable dos_int0 that can hold the address of an interrupt handler.
static void (__interrupt __far* dos_int0)(void);

/**
 * In Watcom C, __interrupt declares a function as an interrupt handler.
 * The compiler generates an IRET return rather than an ordinary RET,
 * and saves/restores the registers required by the interrupt calling convention.
 */
void __interrupt __far fxp_int0_handler() {
    __asm {
        .8086
        shl     cx, 1   ; 8086 limitations CX >> 5 for message offset
        shl     cx, 1
        shl     cx, 1
        shl     cx, 1
        shl     cx, 1
        lea     dx, FXP_PANIC_MESSAGES
        add     dx, cx                     ; DX = selected message
        mov     ah, DOS_PRINT_STRING
        int     DOS_SERVICE
        mov     al, DOS_INVALID_DATA
        mov     ah, DOS_TERMINATE_PROCESS_WITH_RETURN_CODE
        int     DOS_SERVICE
    }
}

void fxp_install_int0_handler() {
    dos_int0_handler = dos_get_interrupt_vector(0);
    dos_set_interrupt_vector(0, (void __far*)fxp_int0_handler);
}

void fxp_uninstall_int0_handler() {
    dos_set_interrupt_vector(0, dos_int0_handler);
}

void fxp_panic_int0(int error) {
    __asm {
        .8086
        mov     cx, ax
        int     0
    }
}
