/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.2
 * formatting characters:
 * + 'x'  lower case hexadecimal
 * + 'X'  upper case hexadecimal
 * + 'b'  binary
 * + 'd'  decimal
 * delimiters:
 * + any printable char
 * + '\0' no delimiter
 * bookends lhs and rhs:
 * + any printable char
 * + '\0' no bookend
 */
#ifndef LOG_TOOLS_H
#define LOG_TOOLS_H

#include "../../doslib/src/DOS/dos_memory_types.h"

#include <stddef.h>
#include <stdio.h>

#define LOG_BYTE_DELIM     ' '
#define LOG_BYTES_LHS      ' '
#define LOG_BYTES_RHS      ' '
#define LOG_CHAR_DELIM     '.'
#define LOG_CHARS_LHS      '|'
#define LOG_CHARS_RHS      '|'
#define LOG_PTR_DELIM      ':'

void set_log_stream(FILE* os);

FILE* get_log_stream();

void log_char(char b, char rhs);

void log_hex(char b, char rhs, const char* hex_chars);

void log_bin(char b, char rhs);

void log_byte(char b, char rhs, char fmt);

void log_word(short w, char rhs, char fmt);

void log_ptr(const void* p, char delim, char rhs, char fmt);

const void* log_chars(const char* b, size_t n, char delim, char lhs, char rhs);

const void* log_bytes(const char* b, size_t n, char delim, char lhs, char rhs);

const void* log_para(const char* para);

const void* log_mem(const char* p, size_t paragraphs);

const void* log_block(dos_mem_block_t mem_block);

#endif
