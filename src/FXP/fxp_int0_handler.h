/**
 * Copyright (C) 2026 Dr Jeremy Thornton
 * DOSFXP - Fast Fixed-Point Number Representation
 * @brief IDIV DIV INT 0 interrupt handler
 *
 * The 8086 triggers interrupt 0 (Divide by Zero) when a DIV or IDIV instruction attempts to divide by zero or
 * when the quotient is too large for the destination register, then CPU generates a call for INT 0h handler.
 * The DOSFXP INT 0 handler terminates execution of the current program, displays the message:
 * "Your program caused an fxp_div overflow error...", and transfers control to DOS.
 */
#ifndef FXP_INT0_HANDLER_H
#define FXP_INT0_HANDLER_H

void fxp_install_int0_handler();

void fxp_uninstall_int0_handler();

#endif
