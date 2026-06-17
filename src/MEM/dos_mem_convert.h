#ifndef DOS_MEM_CONVERT_H
#define DOS_MEM_CONVERT_H

    static inline unsigned long dos_ptr_to_flat(void *p) {
        return ((unsigned long)p >> 16 << 4) + ((unsigned long)p & 0xFFFF);
    }

    static inline void* dos_flat_to_ptr(unsigned long flat) {
        unsigned short seg = (unsigned short)(flat >> 4);
        unsigned short off = (unsigned short)(flat & 0x0F);
        return (void *)((unsigned long)seg << 16 | off);
    }

#endif
