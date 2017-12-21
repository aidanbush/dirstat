/* Author: Aidan
 * Date: Dec. 19, 17
 * File: tests.c
 * Description: testing file for dirstat project
 */

/* stardard library includes*/
#include <stdio.h>

/* project includes */
#include "file_info.h"
#include "open_dirs.h"
#include "search.h"

int test_file_info() {
    int fails = 0;

    //test single file
    file_s *file = get_info(".");
    if (file == NULL) {
        fprintf(stderr, "failed to open file\n");
        fails++;
    }

    if (file == NULL || file->parent != NULL) {
        fprintf(stderr, "failed to set parent to NULL\n");
        fails++;
    }

    if (file == NULL || file->num_files != 0) {
        fprintf(stderr, "failed to set num_files\n");
        fails++;
    }

    if (file == NULL || file->total_num_files != 0) {
        fprintf(stderr, "failed to set total_num_files\n");
        fails++;
    }

    if (file == NULL || file->size == 0) {
        fprintf(stderr, "failed to set size\n");
        fails++;
    }

    if (file == NULL || file->size != file->total_size) {
        fprintf(stderr, "failed to set total_size\n");
        fails++;
    }

    //test adding a file
    if (add_file(file, ".") == 0) {
        fprintf(stderr, "failed to add file\n");
        fails++;
    }

    if (file == NULL || file->num_files != 1) {
        fprintf(stderr, "failed to set num_files after adding a file\n");
        fails++;
    }

    if (file == NULL || file->total_num_files != 1) {
        fprintf(stderr, "failed to set total_num_files after adding a file\n");
        fails++;
    }

    if (file == NULL || file->num_files != file->total_num_files) {
        fprintf(stderr, "num_files != total_num_files\n");
        fails++;
    }

    free_file_s(file);

    return fails;
}

int test_open_dirs() {
    int fails = 0;

    //open initial dir
    file_s *file = get_info(".");
    if (file == NULL) {
        fails++;
    } else if (open_dir(file) == 0) {
        fails++;
    }

    //test if . is in there

    free_file_s(file);

    return fails;
}

int test_search() {
    int fails = 0;

    file_s *file = search(".");
    if (file == NULL) {
        fails++;
    }

    if (file == NULL || file->num_files == 0) {
        fprintf(stderr, "failed to search multiple files\n");
        fails++;
    }

    calculate_file_stats(file);

    printf("total files: %d\n", file->total_num_files);

    if (file == NULL || file->num_files >= file->total_num_files) {
        fprintf(stderr, "num_files >= total_num_files\n");
        fails++;
    }

    if (file == NULL || file->size >= file->total_size) {
        fprintf(stderr, "size >= total_size\n");
        fails++;
    }

    free_file_s(file);

    return fails;
}

int main(int argc, char const *argv[]) {
    int t_fails = 0;
    t_fails += test_file_info();
    t_fails += test_open_dirs();
    t_fails += test_search();

    if (t_fails > 0)
        printf("Total test fails: %d\n", t_fails);
    else
        printf("All tests PASSED!\n");

    return 0;
}