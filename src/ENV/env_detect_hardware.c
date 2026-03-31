#include "env_detect_hardware.h"
#include "env_constants.h"

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

unsigned char env_detect_IBM() {
    unsigned char id = env_read_model_byte();
    if(id >= IBM_MODEL_PS2_70 && id <=IBM_MODEL_XT)
        return id - IBM_MODEL_PS2_70 + 1;
    return 0;
}
