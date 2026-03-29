#ifndef TEST_GENV_H
#define TEST_GENV_H

#include <stdio.h>
#include "../CGA/cga_detect_adapter.h"
#include "../EGA/ega_detect_adapter.h"
#include "../HGA/hga_detect_adapter.h"
#include "../VGA/vga_detect_adapter.h"
#include "../ENV/env_detect_graphics_adapter.h"

void test_genv() {
    //printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[cga_detect_adapter()]);
    //printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[ega_detect_adapter()]);
    //printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[vga_detect_adapter()]);
    //printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[hga_detect_adapter()]);
    printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[env_detect_colour_adapter()]);
    printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[env_detect_monochrome_adapter()]);
}

#endif
