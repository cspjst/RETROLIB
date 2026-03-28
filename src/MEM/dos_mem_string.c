#include "dos_mem_string.h"

int mem_cmp(const void *s1, const void *s2, int n) {
    if (!s1 || !s2) return (s1 > s2) - (s1 < s2);

    const unsigned char* _s1 = (const unsigned char*)s1;
    const unsigned char* _s2 = (const unsigned char*)s2;

    while (n--) {
        if (*_s1 != *_s2) return *_s1 - *_s2;
        _s1++;
        _s2++;
    }

    return 0;
}

void* mem_mem(const void* haystack, int hsize, const void* needle, int nsize) {
    if (!haystack || !needle) return (void*)0;
    if (nsize == 0) return (void*)haystack;         // as per POSIX return haystack pointer if needle is empty
    if (hsize == 0 || nsize > hsize) return (void*)0;

    const unsigned char* hay = (const unsigned char*)haystack;
    const unsigned char* ndl = (const unsigned char*)needle;

    while(nsize <= hsize--) {
        if(mem_cmp(hay, ndl, nsize) == 0) return (void*)hay;
        hay++;
    }

    return (void*)0;
}
