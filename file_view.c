/*
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "file_info.h"
#include "open_directory.h"
#include "file_search.h"

/* ncurses includes */
#include "file_ncurses.h"

int v;

void create_tree(file_struct*);
void print_usage(char*);

int main(int argc, char **argv) {
    char* filename = NULL;
    char* path = NULL;

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
                filename = optarg;
                break;
            case 'p':
                path = optarg;
                break;
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }
    if (!headless)
        init_ncurses();

    //if filename or path are NULL use defaults
    if (filename == NULL || path == NULL) {
        if (filename != NULL)
            free(filename);
        else if (path != NULL)
            free(path);

        filename = malloc(sizeof(char) *2);
        if (filename == NULL)
            return 1;
        strcpy(filename, "/");

        path = malloc(sizeof(char) *2);
        if (path == NULL) {
            free(filename);
            return 1;
        }
        strcpy(path, "/");
    }

    // start generating file tree
    file_struct* file_s = get_file(filename, path);

    if (file_s == NULL) {
        fprintf(stderr, "error in getting file info\n");
        free(filename);
        free(path);
        return 1;
    }

    create_tree(file_s);

    //print debug info
    if (v >= 1) {
        pre_string* str;
        str = create_pre_string(5);
        debug_print_files(file_s, str);
        free_pre_string(str);
    }

    if (!headless)
        display_view(file_s);

    delete_files(file_s);

    free(filename);
    free(path);
    
    if (!headless)
        exit_ncurses();

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
           "    -n\tused to disable ncurses view\n"
           "    starting file options ( must use both or ignored )\n"
           "    -f\tfilename option must include a filename argument\n"
           "    -p\tpath options must include a path argument\n"
           "      \tthe path must include the filename and not end with a \'\\\'\n"
            , p_name);
}
