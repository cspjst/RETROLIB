#ifndef TEST_MDA_H
#define TEST_MDA_H

#include <stdio.h>
#include <assert.h>

#include "../MDA/mda_detect_adapter.h"
#include "../ENV/env_graphics_constants.h"

void test_detect() {
    assert(mda_detect_adapter() == ENV_VIDEO_ADAPTER_MDA);
}

void test_mda() {
    test_detect();
}

#endif
