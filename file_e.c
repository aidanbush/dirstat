#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "file_info.h"

int main(int argc, char const *argv[]) {
    char* filename = "/home/aidan/Documents/360/hash.c";
    file_struct* file_s = get_file(filename);

    if (file_s == NULL) {
        fprintf(stderr, "error in getting file info\n");
        return 1;
    }

    debug_print_file_s(file_s);
    return 0;
}