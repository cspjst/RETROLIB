/**
 * @brief pretty printing to stderr
 * @note needs only fputc to be defined
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
#ifndef DOS_MEMORY_TOOLS_H
#define DOS_MEMORY_TOOLS_H

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

const void* log_paragraph(const char* para);

const void* log_memory(dos_mem_block_t mem_block, size_t paragraphs);

#endif
