/**
 * @author      Jeremy Simon Thornton
 * @copyright   2026 Jeremy Simon Thornton
 * @version     0.1.2
 */
#include "log_tools.h"
#include "../../doslib/src/DOS/dos_memory_constants.h"
#include "../../doslib/src/DOS/dos_memory_types.h"
#include "../../doslib/src/DOS/dos_services_types.h"
#include "../../doslib/src/DOS/dos_services.h"
#include "../../doslib/src/DOS/dos_services_tools.h"

static const char LOG_UPPER_CASE_HEX[] = "0123456789ABCDEF";
static const char LOG_LOWER_CASE_HEX[] = "0123456789abcdef";

static FILE* ostream = NULL;

void set_log_stream(FILE* os) { ostream = os; }

FILE* get_log_stream() { return ostream; }

void log_char(char b, char rhs) {
    if (b < 32 || b > 126) fputc('.', ostream);
    else fputc(b, ostream);
    if(rhs) fputc(rhs, stderr);
}

void log_hex(char b, char rhs, const char* hex_chars) {
    fputc(hex_chars[(b >> 4) & 0x0F], ostream);
    fputc(hex_chars[b & 0x0F], ostream);
    if(rhs) fputc(rhs, ostream);
}

void log_bin(char b, char rhs) {
    for (int i = 7; i >= 0; i--)
        fputc((b & (1 << i)) ? '1' : '0', ostream);
    if (rhs) fputc(rhs, ostream);
}

void log_byte(char b, char rhs, char fmt) {
    switch(fmt) {
        case 'b': //log_binary()
        case 'x': log_hex(b, rhs, LOG_LOWER_CASE_HEX);
        case 'X':
        default: log_hex(b, rhs, LOG_UPPER_CASE_HEX);
    }
}

void log_word(short w, char rhs, char fmt) {
    dos_word_t m;
    m.word = w;
    log_byte(m.bytes[1], 0, fmt);
    log_byte(m.bytes[0], rhs, fmt);
}

void log_ptr(const void* p, char delim, char rhs, char fmt) {
    dos_address_t addr;
    addr.ptr = (void*)p;
    log_word(addr.words[1], delim, fmt);
    log_word(addr.words[0], rhs, fmt);
}

const void* log_chars(const char* b, size_t n, char delim, char lhs, char rhs) {
    if (lhs) fputc(lhs, ostream);
    while(n--) log_char(*b++, delim);
    if (rhs) fputc(rhs, ostream);
    return (void*)b;
}

const void* log_bytes(const char* b, size_t n, char delim, char lhs, char rhs) {
    if (lhs) fputc(lhs, ostream);
    while(n--) log_byte(*b++, delim, 'X');
    if (rhs) fputc(rhs, ostream);
    return (void*)b;
}

const void* log_para(const char* para) {
    log_ptr(para, LOG_PTR_DELIM, LOG_BYTES_RHS, 'X');
    log_bytes(para, DOS_PARAGRAPH_SIZE, LOG_BYTE_DELIM, LOG_BYTES_LHS, LOG_BYTES_RHS);
    log_chars(para , DOS_PARAGRAPH_SIZE, 0, LOG_CHARS_LHS, LOG_CHARS_RHS);
    fputc('\n', ostream);
    return (void*)(para + DOS_PARAGRAPH_SIZE);
}

const void* log_mem(const char* p, size_t paragraphs) {
    while(paragraphs--)
       p = log_para((const void*)p);
    return (void*)p;
}

const void* log_mem_block(dos_mem_block_t mem_block) {
    if(mem_block.begin.ptr >= mem_block.end.ptr) return NULL;
    const char* p = (char*)mem_block.begin.ptr;
    const char* end = (char*)mem_block.end.ptr;
    while(p < end)
       p = log_para((const void*)p);
    return (void*)p;
}

void log_timestamp() {
    dos_date_t d;
    dos_time_t t;
    char stamp[19]; // YYYY-MM-DD HH:MM:SS
    dos_get_date(&d);
    dos_get_time(&t);
    char* p = dos_date_to_str(&d, stamp, '-');
    *p++ = ' ';
    dos_time_to_str(&t, p, ':');
    log_chars(stamp, 19, 0,  0, ' ');
}

void log_mcb(const dos_mcb_t* mcb) {
    dos_address_t addr;
    addr.ptr = (void*)mcb;
    fprintf(ostream, "MCB @0x%04X ", addr.segoff.segment);
    fprintf(ostream,"ID 0x%02X %c ", mcb->chain_id, mcb->chain_id);
    fprintf(ostream,"PID 0x%04X ", mcb->pid);
    fprintf(ostream,"Size %5uparas %lubytes\n", mcb->block_size, mcb->block_size * 16UL);
}
