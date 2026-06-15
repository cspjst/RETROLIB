#ifndef LIB_CONVERT_H
#define LIB_CONVERT_H

#include <stdbool.h>

bool lib_str_is_hex(const char* s);

bool lib_str_to_uint(const char* s, unsigned int* u);

#endif
