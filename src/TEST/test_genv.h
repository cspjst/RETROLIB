#ifndef TEST_GENV_H
#define TEST_GENV_H

#include <stdio.h>
#include "../CGA/cga_detect_adapter.h"
#include "../EGA/ega_detect_adapter.h"
#include "../HGA/hga_detect_adapter.h"
#include "../VGA/vga_detect_adapter.h"
#include "../ENV/env_detect_graphics_adapter.h"
#include "../ENV/env_emulation.h"


void test_genv() {
    switch((int)env_detect_DOSBox()) {
        case DOS_EMULATOR_DOSBOX: printf("DOSBox/DOSBox Staging emulation unreliable video adapter reporting!\n");
        case DOS_EMULATOR_DOSBOX_X: printf("DOSBox-X emulation reliable video adapter reporting!\n");
        default:
            printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[env_detect_colour_adapter()]);
            printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[env_detect_monochrome_adapter()]);
    }


}

#endif
