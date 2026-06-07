#ifndef DOS_MEM_DUMP_H
#define DOS_MEM_DUMP_H

#include <stdio.h>

/**
 * prints 16 bytes as hexadecimal pairs followed by ASCII characters (0x20-0x7F) or '.' for
 * non-printable bytes, or ' ' for padding if block is not a multiple of 16.
 * decrements *nchars by bytes printed - caller loops until *nchars == 0
 */
void dos_mem_dump_paragraph(FILE* ostream, char** mem_block, int* nchars);

void dos_mem_dump_block(FILE* ostream,char* begin, char* end);

#endif
