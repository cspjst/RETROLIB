#include "dos_environment.h"
#include "dos_environment_constants.h"

#include "../STD/dos_string.h"

dos_emulator_t dos_environment_is_DOSBox() {
    void* p = memmem(MEM_BLOCK_ROM_BIOS.begin.ptr, SEARCH_SIZE_DOSBOX, SEARCH_DOSBOX, sizeof(SEARCH_DOSBOX));
    if(!p) return DOS_EMULATOR_NONE;
    if(memmem(MEM_BLOCK_ROM_BIOS.begin.ptr, SEARCH_SIZE_DOSBOX, SEARCH_DOSBOX_X, sizeof(SEARCH_DOSBOX_X))) {
        return DOS_EMULATOR_DOSBOX_X;
    }
    return DOS_EMULATOR_DOSBOX;
}
