#ifndef DOS_MEMMEM_H
#define DOS_MEMMEM_H

#include <stddef.h>

void* memmem(const void* haystack, size_t hsize, const void* needle, size_t nsize);

#endif
