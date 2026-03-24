#ifndef __LARGE__
    #error "This module requires large memory model (ie far data pointers)"
#endif

#include "STD/dos_stdio.h"

//#include "TEST/test_bios.h"
//#include "TEST/test_dos_memory.h"
//#include "TEST/test_dos_services.h"
//#include "TEST/test_dos_files.h"
#include "TEST/test_env.h"
//#include "TEST/test_stdio.h"
//#include "TEST/test_string.h"
//#include "TEST/test_files.h"
//#include "TEST/test_stdlib.h"
//#include "TEST/dos_stdio.h"

int main() {

#ifdef USE_DOSLIBC
    printf("USE_DOSLIBC Test Harness\n");
#else
    printf("Test Harness\n");
#endif
    // BIOS
    //test_bios_memory();
    //test_bios_keys();

    // DOS
    //test_dos_memory();
    //test_dos_services();
    //test_dos_files();

#ifdef USE_DOSLIBC
    printf("DOSLIBC Test Harness\n");
    // STD
    // test_stdio();
    // test_string();
    // test_files();
    // test_stdlib();
#else
    printf("ERROR unable to test DOSLIBC no definition USE_DOSLIBC \n");
#endif

    // ENV
    test_env();

}
