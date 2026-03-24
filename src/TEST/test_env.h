#ifndef TEST_ENV_H
#define TEST_ENV_H


#ifdef USE_DOSLIBC
    #include "../STD/dos_stdio.h"
    #include "../STD/dos_assert.h"
#else
    #include <stdio.h>
    #include <assert.h>
#endif

#include "../ENV/dos_environment_types.h"
#include "../ENV/dos_environment_constants.h"
#include "../ENV/dos_environment.h"
#include "../DUMP/dos_dump_tools.h"
#include "../DOS/dos_memory_constants.h"

void test_types() {
    printf("Testing DOS environment types...\n");

    assert(MEM_BLOCK_ROM_BIOS.begin.segoff.segment == ROM_SEG_BEGIN);
    assert(MEM_BLOCK_ROM_BIOS.begin.segoff.offset == ROM_OFF_BEGIN);
    assert(MEM_BLOCK_ROM_BIOS.end.segoff.segment == ROM_SEG_END);
    assert(MEM_BLOCK_ROM_BIOS.end.segoff.offset == ROM_OFF_END);

    dos_set_dump_stream(stderr);
    assert(dos_dump_memory(MEM_BLOCK_ROM_BIOS, 16));
    assert(dos_dump_memory(MEM_BLOCK_IVT, 1));

    switch((int)dos_environment_is_DOSBox()) {
        case DOS_EMULATOR_NONE: printf("No DOSBox emulator detected.\n"); break;
        case DOS_EMULATOR_DOSBOX: printf("DOSBox/DOSBox Staging emulation!\n"); break;
        case DOS_EMULATOR_DOSBOX_X: printf("DOSBox-X emulation!\n"); break;
    }
}

void test_env() {
    test_types();
}

#endif
