/**
* @author      Jeremy Simon Thornton
* @copyright   2026 Jeremy Simon Thornton
* @version     0.2.0
*/
#ifndef DOS_MEM_STRING_H
#define DOS_MEM_STRING_H

/**
 * compare two blocks of memory, returning zero if they are identical,
 * or a non-zero value indicating which block is greater.
 */
 int mem_cmp(const void *s1, const void *s2, int n);

/**
 * like GLIBC memmem - locate a substring
 * finds the start of the first occurrence of the substring needle of size nsize
 * in the memory area haystack of size hsize.
 * @return  pointer to the beginning of the substring,
 *          or NULL if the substring is not found
 */
void* mem_mem(const void* haystack, int hsize, const void* needle, int nsize);

#endif
