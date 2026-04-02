#include "cga_hi_res_blt.h"

#include "cga_constants.h"

void cga_hi_res_fill_vram(const char* data) {
    __asm {
        .8086
        push    ds
        pushf

        lds     si, data
        les     di, CGA_VRAM_PTR
        cld
        

        popf
        pop     ds
    }
}
