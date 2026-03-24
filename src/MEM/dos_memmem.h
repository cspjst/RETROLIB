/**
* @author      Jeremy Simon Thornton
* @copyright   2026 Jeremy Simon Thornton
* @version     0.1.0
*/
#ifndef DOS_MEMMEM_H
#define DOS_MEMMEM_H

#include <stddef.h>

/**
 * GLIBC memmem - locate a substring
 * finds the start of the first occurrence of the substring needle of size nsize
 * in the memory area haystack of size hsize.
 * @return  pointer to the beginning of the substring,
 *          or NULL if the substring is not found
 */
void* memmem(const void* haystack, size_t hsize, const void* needle, size_t nsize);

#endif
