/*
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "file_info.h"
#include "open_directory.h"
#include "file_search.h"

int v;

void file_getopt(int, char**);

int main(int argc, char **argv) {
    file_getopt(argc, argv);

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

    //generate tree
    search(file_s);

    //generate calulations
    calculate_stats(file_s);

    //sort tree
    sort_files(file_s);

    //print debug info
    pre_string* str;
    str = create_pre_string(5);
    debug_print_files(file_s, str);
    free_pre_string(str);

    delete_files(file_s);

    free(filename);
    free(path);
    return 0;
}

void file_getopt(int argc, char **argv) {
    char c;

    v = 0;

    while ((c = getopt(argc, argv, "vh")) != -1) {
        switch (c){
            case 'v':
                v++;
                break;
            case 'h':
                exit(0);
            default:
                exit(1);
        }
    }
}
