#include "dos_mem_dump.h"

void dos_mem_dump_paragraph(FILE* ostream, char** mem_block, int* nchars) {
    char* p = *mem_block;
    int i;
    int n = *nchars > 16 ? 16 : *nchars;
    fprintf(ostream, "%p  ", p);
    for(i = 0; i < 16; i++)
        fprintf(ostream, i < n ? "%02X " : "   ", (unsigned char)*p++);
    fprintf(ostream, " ");
    p = *mem_block;
    for(i = 0; i < 16; i++)
        fprintf(ostream, "%c", i < n ? (*p >= 0x20 && *p <= 0x7F ? *p++ : (p++, '.')) : ' ');
    fprintf(ostream, "\n");
    *mem_block += n;
    *nchars -= n;
}

void dos_mem_dump_block(FILE* ostream, char* begin, char* end) {
    int nchars = end - begin;
    while(nchars > 0)
        dos_mem_dump_paragraph(ostream, &begin, &nchars);
}
