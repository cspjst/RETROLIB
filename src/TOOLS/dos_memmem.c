#include "dos_memmem.h"

#include <string.h>

void* memmem(const void* haystack, size_t hsize, const void* needle, size_t nsize) {
    if (!haystack || !needle) return NULL;
    if (nsize == 0) return (void*)haystack;         // as per POSIX return haystack pointer if needle is empty
    if (hsize == 0 || nsize > hsize) return NULL;

    const unsigned char* hay = (const unsigned char*)haystack;
    const unsigned char* ndl = (const unsigned char*)needle;

    while(nsize <= hsize--) {
        if(memcmp(hay, ndl, nsize) == 0) return (void*)hay;
        hay++;
    }

    return NULL;
}
