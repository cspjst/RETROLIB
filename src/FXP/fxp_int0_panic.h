/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 *
 * Provides the panic message values for CX prior to calling INT 0
 * @note Requies void fxp_install_int0_handler() to have been called at set up
 * @see fxp_int0_handler.h
 */
#ifndef FXP_INT0_PANIC_H
#define FXP_INT0_PANIC_H

#define FXP_PANIC_DIV   0
#define FXP_PANIC_SQRT  1
#define FXP_PANIC_EXP   2
#define FXP_PANIC_FIX   3

#endif
