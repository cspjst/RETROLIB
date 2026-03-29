/**
 * @author      Jeremy Simon Thornton
 * @copyright   2024 Jeremy Simon Thornton
 * @version     0.1.0
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
