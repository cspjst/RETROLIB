#include "env_detect_hardware.h"

unsigned char env_read_model_byte() {
    unsigned char id;
    __asm {
        push    es
        mov     ax, 0xF000
        mov     es, ax
        mov     al, [es:0xFFFC]
        pop     es
        mov     id, al
    }
    return id;
}
