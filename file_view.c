#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "file_info.h"
#include "open_directory.h"
#include "file_search.h"

int main(int argc, char const *argv[]) {
    char* filename = malloc(sizeof(char) * 250);
    char* path = malloc(sizeof(char) * 250);
    printf("enter filename: ");
    scanf("%s", filename);
    printf("enter pathname: ");
    scanf("%s", path);

    file_struct* file_s = get_file(filename, path);

    if (file_s == NULL) {
        fprintf(stderr, "error in getting file info\n");
        return 1;
    }

    search(file_s);
    debug_print_files(file_s, 0);
    delete_files(file_s);

    /*
    debug_print_file_s(file_s);
    open_dir(file_s);
    fprintf(stderr, "reprinting debug file struct\n");
    debug_print_file_s(file_s);
    */
    return 0;
}
