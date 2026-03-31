/**
* @author      Jeremy Simon Thornton
* @copyright   2026 Jeremy Simon Thornton
* @version     0.1.0
*/
#include "../../doslib/src/DOS/dos_memory_types.h"

const dos_mem_block_t MEM_BLOCK_IVT = {(void*)0x00000000, (void*)0x000003FF};
const dos_mem_block_t MEM_BLOCK_BDA = {(void*)0x00400000, (void*)0x004000FF};
const dos_mem_block_t MEM_BLOCK_VIDEO_MONO_PAGE_0 = {(void*)0xB0000000, (void*)0xB0000FFF};
const dos_mem_block_t MEM_BLOCK_VIDEO_COLOUR_PAGE_0 = {(void*)0xB8000000, (void*)0xB8000FFF};
const dos_mem_block_t MEM_BLOCK_ROM_BIOS = {(void*)0xFE000000, (void*)0xF000FFFF};

const char SEARCH_DOSBOX[] = "DOSBox";
const char SEARCH_DOSBOX_X[] = "DOSBox-X";

const char env_names_dosbox[3][18] = {
    "No DOSBox Detected",
    "DOSBox or Staging",
    "DOSBox-X"
};

const char env_names_ibm_model[9][20] = {
    "Unknown",
    "IBM PS/2 70",
    "IBM PS/2 50",
    "IBM PS/2 30",
    "IBM 5140 Convertible",
    "IBM 5170 AT",
    "IBM Unused",
    "IBM 5160 XT",
    "IBM 5150 PC"
};
