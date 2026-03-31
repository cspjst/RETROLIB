/**
* @author      Jeremy Simon Thornton
* @copyright   2026 Jeremy Simon Thornton
* @version     0.1.0
*/
#ifndef DOS_ENVIRONMENT_CONSTANTS_H
#define DOS_ENVIRONMENT_CONSTANTS_H

#include "../../doslib/src/DOS/dos_memory_types.h"

#define ROM_BIOS_SEG_BEGIN   0xFE00
#define ROM_BIOS_OFF_BEGIN   0x0000
#define ROM_BIOS_SEG_END     0xF000
#define ROM_BIOS_OFF_END     0xFFFF

#define ROM_BIOS_OFF_MODEL_BYTE 0xFFFC  // IBM hardware using the ROM BIOS Model Byte (2 bytes before the end of the ROM segment)

extern const dos_mem_block_t MEM_BLOCK_IVT;
extern const dos_mem_block_t MEM_BLOCK_BDA;
extern const dos_mem_block_t MEM_BLOCK_VIDEO_MONO_PAGE_0;
extern const dos_mem_block_t MEM_BLOCK_VIDEO_COLOUR_PAGE_0;
extern const dos_mem_block_t MEM_BLOCK_ROM_BIOS;

#define SEARCH_SIZE_DOSBOX  128
extern const char SEARCH_DOSBOX[6];
extern const char SEARCH_DOSBOX_X[8];

typedef enum {
    DOS_EMULATOR_NONE = 0,
    DOS_EMULATOR_DOSBOX,
    DOS_EMULATOR_DOSBOX_X
} env_emulator_t;
extern const char env_names_dosbox[3][18];

typedef enum {
    IBM_MODEL_PS2_70 = 0xF8,
    IBM_MODEL_PS2_50,           // 0xF9
    IBM_MODEL_PS2_30,           // 0xFA
    IBM_MODEL_CONVERTIBLE,      // 0xFB
    IBM_MODEL_AT,               // 0xFC
    IBM_UNUSED,                 // 0xFD
    IBM_MODEL_PC,               // 0xFE
    IBM_MODEL_XT                // 0xFF
} env_ibm_model_id_t;
extern const char env_names_ibm_model[9][20];

#endif
