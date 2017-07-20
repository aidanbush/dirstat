#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "file_info.h"
#include "open_directory.h"

int main(int argc, char const *argv[]) {
    char* filename = malloc(sizeof(char) * 250);
    scanf("%s", filename);
    file_struct* file_s = get_file(filename);

    if (file_s == NULL) {
        fprintf(stderr, "error in getting file info\n");
        return 1;
    }

    debug_print_file_s(file_s);
    open_dir(file_s);
    fprintf(stderr, "reprinting debug file struct\n");
    debug_print_file_s(file_s);
    return 0;
}