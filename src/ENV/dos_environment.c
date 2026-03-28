/**
* @author      Jeremy Simon Thornton
* @copyright   2026 Jeremy Simon Thornton
* @version     0.1.0
*/
#include "dos_environment.h"
#include "dos_environment_constants.h"

#include "../MEM/dos_mem_string.h"

dos_emulator_t dos_environment_is_DOSBox() {
    void* p = mem_mem(MEM_BLOCK_ROM_BIOS.begin.ptr, SEARCH_SIZE_DOSBOX, SEARCH_DOSBOX, sizeof(SEARCH_DOSBOX));
    if(!p) return DOS_EMULATOR_NONE;
    if(mem_mem(MEM_BLOCK_ROM_BIOS.begin.ptr, SEARCH_SIZE_DOSBOX, SEARCH_DOSBOX_X, sizeof(SEARCH_DOSBOX_X))) {
        return DOS_EMULATOR_DOSBOX_X;
    }
    return DOS_EMULATOR_DOSBOX;
}
