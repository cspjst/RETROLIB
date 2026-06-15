#include "lib_convert.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const char LIB_HEX_DIGITS[] = "0123456789ABCDEFabcdef";

bool lib_str_is_hex(const char* s) {
    if(strlen(s) > 16) return false;    // i.e. more than 64 bits long long
    const char* p = s;
    while(*p && strchr(LIB_HEX_DIGITS, *p)) p++;
    return *p == '\0';
}

bool lib_str_to_uint(const char* s, unsigned int* u) {
    char *end;
    errno = 0;
    long l = strtol(s, &end, 0);   // sets errno to ERANGE
    if (
        errno ||
        end == s ||
        *end != '\0' ||
        l < 0 ||
        l >= 0x10000
    ) return false;
    *u = (unsigned int)l;
    return true;
}
