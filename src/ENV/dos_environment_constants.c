#include "dos_environment_constants.h"

const dos_mem_block_t MEM_BLOCK_IVT = {(void*)0x00000000, (void*)0x000003FF};
const dos_mem_block_t MEM_BLOCK_BDA = {(void*)0x00400000, (void*)0x004000FF};
const dos_mem_block_t MEM_BLOCK_VIDEO_MONO_PAGE_0 = {(void*)0xB0000000, (void*)0xB0000FFF};
const dos_mem_block_t MEM_BLOCK_VIDEO_COLOUR_PAGE_0 = {(void*)0xB8000000, (void*)0xB8000FFF};
const dos_mem_block_t MEM_BLOCK_ROM_BIOS = {(void*)0xFE000000, (void*)0xF000FFFF};

const char SEARCH_DOSBOX[] = "DOSBox";
const char SEARCH_DOSBOX_X[] = "DOSBox-X";
