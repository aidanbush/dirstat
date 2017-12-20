/* Author: Aidan
 * Date: Dec. 19, 17
 * File: mydirstat.c
 * Description: main file for dirstat tool
 */

/* stardard library includes*/
#include <unistd.h>
#include <libgen.h>
#include <stdio.h>

/* project includes */

void print_usage(char *progname) {
    printf("Usage : %s [ options ]\n"
        "A simple dirstat tool.\n"
        "Options:\n"
        "    -h displays this usage message\n", basename(progname));
}

int main(int argc, char *argv[]) {
    int c;

    while ((c = getopt(argc, argv, "h")) != -1) {
        switch (c) {
            case 'h':
                print_usage(argv[0]);
                break;
            default:
                print_usage(argv[0]);
                break;
        }
    }

    return 0;
}