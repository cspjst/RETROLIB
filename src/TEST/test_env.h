#ifndef TEST_ENV_H
#define TEST_ENV_H

#include <stdio.h>
#include <assert.h>

#include "../ENV/env_constants.h"
#include "../ENV/env_emulation.h"
#include "../ENV/env_detect_crtc.h"
#include "../ENV/env_graphics_constants.h"
#include "../LOG/log_tools.h"
#include "../../doslib/src/DOS/dos_memory_constants.h"

void test_types() {
    printf("Testing DOS environment types...\n");

    assert(MEM_BLOCK_ROM_BIOS.begin.segoff.segment == ROM_SEG_BEGIN);
    assert(MEM_BLOCK_ROM_BIOS.begin.segoff.offset == ROM_OFF_BEGIN);
    assert(MEM_BLOCK_ROM_BIOS.end.segoff.segment == ROM_SEG_END);
    assert(MEM_BLOCK_ROM_BIOS.end.segoff.offset == ROM_OFF_END);

    set_log_stream(stderr);
    assert(log_mem((char*)MEM_BLOCK_ROM_BIOS.begin.ptr, 16));
    assert(log_mem((char*)MEM_BLOCK_IVT.begin.ptr, 1));
    log_timestamp();

    switch((int)env_detect_DOSBox()) {
        case DOS_EMULATOR_NONE: printf("No DOSBox emulator detected.\n"); break;
        case DOS_EMULATOR_DOSBOX: printf("DOSBox/DOSBox Staging emulation!\n"); break;
        case DOS_EMULATOR_DOSBOX_X: printf("DOSBox-X emulation!\n"); break;
    }
    printf("%s\n",env_detect_crtc(crtc_port_CGA) ?"CGA CRTC detected\n" :"NO CGA CRTC detected!\n");
}

void test_env() {
    test_types();
}

#endif
