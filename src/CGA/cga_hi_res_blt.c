#include "cga_hi_res_blt.h"

#include "cga_constants.h"

void cga_hi_res_full_screen_blt(const char* data) {
    __asm {
        .8086
        push    ds
        pushf

        lds     si, data
        les     di, CGA_VRAM_PTR

        popf
        pop     ds
    }
}
