#include "dos_dump_tools.h"
#include "../DOS/dos_memory_constants.h"
#include "../DOS/dos_memory_types.h"

static const char DUMP_UPPER_CASE_HEX[] = "0123456789ABCDEF";
static const char DUMP_LOWER_CASE_HEX[] = "0123456789abcdef";

static FILE* ostream = NULL;

void dos_set_dump_stream(FILE* os) { ostream = os; }

FILE* dos_get_dump_stream() { return ostream; }

void dos_dump_char(char b, char rhs) {
    if (b < 32 || b > 126) fputc('.', ostream);
    else fputc(b, ostream);
    if(rhs) fputc(rhs, stderr);
}

void dos_dump_hex(char b, char rhs, const char* hex_chars) {
    fputc(hex_chars[(b >> 4) & 0x0F], ostream);
    fputc(hex_chars[b & 0x0F], ostream);
    if(rhs) fputc(rhs, ostream);
}

void dos_dump_binary(char b, char rhs) {
    for (int i = 7; i >= 0; i--)
        fputc((b & (1 << i)) ? '1' : '0', ostream);
    if (rhs) fputc(rhs, ostream);
}

void dos_dump_byte(char b, char rhs, char fmt) {
    switch(fmt) {
        case 'b': //dos_dump_binary()
        case 'x': dos_dump_hex(b, rhs, DUMP_LOWER_CASE_HEX);
        case 'X':
        default: dos_dump_hex(b, rhs, DUMP_UPPER_CASE_HEX);
    }
}

void dos_dump_word(short w, char rhs, char fmt) {
    dos_word_t m;
    m.word = w;
    dos_dump_byte(m.bytes[1], 0, fmt);
    dos_dump_byte(m.bytes[0], rhs, fmt);
}

void dos_dump_ptr(const void* p, char delim, char rhs, char fmt) {
    dos_address_t addr;
    addr.ptr = (void*)p;
    dos_dump_word(addr.words[1], delim, fmt);
    dos_dump_word(addr.words[0], rhs, fmt);
}

const void* dos_dump_chars(const char* b, size_t n, char delim, char lhs, char rhs) {
    if (lhs) fputc(lhs, ostream);
    while(n--) dos_dump_char(*b++, delim);
    if (rhs) fputc(rhs, ostream);
    return (void*)b;
}

const void* dos_dump_bytes(const char* b, size_t n, char delim, char lhs, char rhs) {
    if (lhs) fputc(lhs, ostream);
    while(n--) dos_dump_byte(*b++, delim, 'X');
    if (rhs) fputc(rhs, ostream);
    return (void*)b;
}

const void* dos_dump_paragraph(const char* para) {
    dos_dump_ptr(para, DUMP_PTR_DELIM, DUMP_BYTES_RHS, 'X');
    dos_dump_bytes(para, DOS_PARAGRAPH_SIZE, DUMP_BYTE_DELIM, DUMP_BYTES_LHS, DUMP_BYTES_RHS);
    dos_dump_chars(para , DOS_PARAGRAPH_SIZE, 0, DUMP_CHARS_LHS, DUMP_CHARS_RHS);
    fputc('\n', ostream);
    return (void*)(para + DOS_PARAGRAPH_SIZE);
}

const void* dos_dump_memory(dos_mem_block_t mem_block, size_t paragraphs) {
    const char* p = (char*)mem_block.begin.ptr;
    if(mem_block.begin.ptr > mem_block.end.ptr) return NULL;
    while(paragraphs--)
       p = dos_dump_paragraph((const void*)p);
    return (void*)p;
}
