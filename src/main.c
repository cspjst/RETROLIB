#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../doslib/src/DOS/dos_file_services.h"

#define NARGS 2
#define ERROR "%s\n"
#define USAGE "usage: ppm2cga <file.ppm> [file2.ppm ...]\n" \
              "       ppm2cga *.ppm\n"

int main(int argc, char* argv[]) {
    errno = DOS_INVALID_FORMAT;
    if(argc < NARGS) goto error;


    for(int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    dos_dta_t dta;
    dos_dta_t** old_dta;

    errno = dos_get_dta(old_dta);
    if(errno) goto error;

    dos_set_dta(&dta);

    dos_set_dta(*old_dta);
    errno = DOS_SUCCESS;
    return 0;
error:
    fprintf(stderr, ERROR, strerror(errno));
    fprintf(stderr, USAGE);
    return 1;
}
