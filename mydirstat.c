/* Author: Aidan
 * Date: Dec. 19, 17
 * File: mydirstat.c
 * Description: main file for dirstat tool
 */

/* standard library includes*/
#include <unistd.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

/* project includes */
#include "file_info.h"
#include "search.h"

/* prints the usage message for the given program name */
void print_usage(char *progname) {
    printf("Usage : %s [ options ]\n"
        "A simple dirstat tool.\n"
        "Options:\n"
        "    -s dirstat start directory\n"
        "    -h displays this usage message\n", basename(progname));
}

int main(int argc, char *argv[]) {
    int c;
    char *start = "/";

    while ((c = getopt(argc, argv, "hs:")) != -1) {
        switch (c) {
            case 'h':
                print_usage(argv[0]);
                exit(0);
            case 's':
                start = optarg;
                break;
            default:
                print_usage(argv[0]);
                break;
        }
    }

    printf("starting at: %s\n", start);

    file_s * file = search(start);
    print_files(file);
    free_file_s(file);

    return 0;
}