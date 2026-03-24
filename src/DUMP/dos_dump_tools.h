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

#include "../DOS/dos_memory_types.h"

#include "../STD/dos_stddef.h"
#include "../STD/dos_stdio.h"

#define DUMP_BYTE_DELIM     ' '
#define DUMP_BYTES_LHS      ' '
#define DUMP_BYTES_RHS      ' '
#define DUMP_CHAR_DELIM     '.'
#define DUMP_CHARS_LHS      '|'
#define DUMP_CHARS_RHS      '|'
#define DUMP_PTR_DELIM      ':'

void dos_set_dump_stream(FILE* os);

FILE* dos_get_dump_stream();

void dos_dump_char(char b, char rhs);

void dos_dump_hex(char b, char rhs, const char* hex_chars);

void dos_dump_binary(char b, char rhs);

void dos_dump_byte(char b, char rhs, char fmt);

void dos_dump_word(short w, char rhs, char fmt);

void dos_dump_ptr(const void* p, char delim, char rhs, char fmt);

const void* dos_dump_chars(const char* b, size_t n, char delim, char lhs, char rhs);

const void* dos_dump_bytes(const char* b, size_t n, char delim, char lhs, char rhs);

const void* dos_dump_paragraph(const char* para);

const void* dos_dump_memory(dos_mem_block_t mem_block, size_t paragraphs);

#endif
