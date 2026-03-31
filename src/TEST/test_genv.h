#ifndef TEST_GENV_H
#define TEST_GENV_H

#include <stdio.h>
#include "../CGA/cga_detect_adapter.h"
#include "../EGA/ega_detect_adapter.h"
#include "../HGA/hga_detect_adapter.h"
#include "../VGA/vga_detect_adapter.h"
#include "../ENV/env_detect_graphics_adapter.h"
#include "../ENV/env_detect_emulation.h"
#include "../ENV/env_video_mode.h"

void test_genv() {
    switch((int)env_detect_DOSBox()) {
        case DOS_EMULATOR_DOSBOX:
            printf("DOSBox/DOSBox Staging emulation unreliable video adapter reporting!\n");
            printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[env_detect_colour_adapter()]);
            printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[env_detect_monochrome_adapter()]);
            break;
        case DOS_EMULATOR_DOSBOX_X: printf("DOSBox-X emulation reliable video adapter reporting!\n");
        default:
            printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[env_detect_colour_adapter()]);
            printf("%s\n", ENV_VIDEO_ADAPTER_NAMES[env_detect_monochrome_adapter()]);
    }
    printf("test video modes?");
    if(getchar() == 'y') {
        bios_video_mode_t m = env_get_video_mode();
        printf("video mode = %s\n", bios_video_mode_names[m]);
        for(int i = 0; i < 0x14; ++i) {
            getchar();
            printf("attempt video mode = %s\n", bios_video_mode_names[(bios_video_mode_t)i]);
            printf("video old mode = %s\n", bios_video_mode_names[env_set_video_mode((bios_video_mode_t)i)]);
            printf("video new mode = %s\n", bios_video_mode_names[env_get_video_mode()]);
        }
        env_set_video_mode(m);
        printf("restored video mode = %s\n", bios_video_mode_names[env_get_video_mode()]);
    }
}

#endif
