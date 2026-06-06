#include <stdio.h>
#include <string.h>

#include "../doslib/src/DOS/dos_file_constants.h"
#include "../doslib/src/DOS/dos_error.h"
#include "../doslib/src/DOS/dos_error_codes.h"

#define USAGE "usage: cgashift <file.cga> [file2.cga ...]\n" \
              "       cgashift *.cga\n"

int main(int argc, char* argv[]) {
    switch(argc) {
        case 3:
            if(strlen(argv[1]) < DOS_MAX_PATH) dos_perror("DOS_MAX_PATH ", DOS_INVALID_FORMAT);
            //assert(strlen(argv[2])<=DOS_MAX_PATH);
            //return do_convert(argv[1], argv[2]);
        case 2:
            //assert(strlen(argv[1])<=DOS_MAX_PATH);
            //return do_wildcard(argv);
        default:
            dos_perror("argc ", DOS_INVALID_FORMAT);
            fprintf(stderr, USAGE);
            return 1;

}
