/**
 * @author      Jeremy Simon Thornton
 * @copyright   2024, 2026 Jeremy Simon Thornton
 * @version     0.1.1
 */
#include "cga_detect_adapter.h"
#include "../ENV/env_graphics_constants.h"
#include "../ENV/env_detect_crtc.h"

unsigned char cga_detect_adapter() {
    if (env_detect_crtc(crtc_port_CGA)) {
        return ENV_VIDEO_ADAPTER_CGA;
    }
    return ENV_VIDEO_ADAPTER_UKNOWN;
}
