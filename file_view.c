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

int file_getopt(int, char**);
void create_tree(file_struct*);
void print_usage(char*);

int main(int argc, char **argv) {
    int headless = file_getopt(argc, argv);

    char* filename = malloc(sizeof(char) * 250);
    char* path = malloc(sizeof(char) * 250);

    /*
    if (!headless)
        ncureses_input(filename, path);
    */

    printf("enter filename: ");
    scanf("%s", filename);
    printf("enter pathname: ");
    scanf("%s", path);

    file_struct* file_s = get_file(filename, path);

    if (file_s == NULL) {
        fprintf(stderr, "error in getting file info\n");
        return 1;
    }

    create_tree(file_s);

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

void create_tree(file_struct* file_s) {
    //generate tree
    search(file_s);

    //generate calulations
    calculate_stats(file_s);

    //sort tree
    sort_files(file_s);
}

void print_usage(char* p_name) {
    printf("usage : %s [ options ]\n"
           "Used to view and analize files.\n\n"
           "Options:\n"
           "    -h\tdisplays this help message and exits\n"
           "    -v\tdisplays extra ( verbose ) debbugging information\n"
           "      \t(multiple -v options increase verbosity)\n"
           "    -n\tused to disable ncurses view\n", p_name);
}

int file_getopt(int argc, char **argv) {
    char c;

    v = 0;
    int headless = 0;

    while ((c = getopt(argc, argv, "vhnf:p:")) != -1) {
        switch (c){
            case 'v':
                v++;
                break;
            case 'h':
                print_usage(argv[0]);
                exit(0);
            case 'n':
                headless = 1;
                break;
            case 'f':
                fprintf(stderr, "f option not yet implemented\narg: %s\n",
                        optarg);
                break;
            case 'p':
                fprintf(stderr, "p option not yet implemented\narg: %s\n",
                        optarg);
                break;
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }
    return headless;
}
