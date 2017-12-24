/* Author: Aidan
 * Date: Dec. 19, 17
 * File: search.c
 * Description: file for recursively searching though all the directories
 */

/* stardard library includes*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* project includes */
#include "search.h"
#include "open_dirs.h"
#include "file_info.h"

/* prints a files with the given prestring first */
static void print_file_search(file_s *file, char *pre_string, int strlen) {
    for (int i = 0; i < strlen; i++)
        printf("%c", pre_string[i]);

    printf("%s:%ld:%ld\n", file->name, file->total_size, file->size);
}

/* resizes the prestring */
static int resize_pre_string(int *len, char **pre_string) {
    *len *= 2;
    char *tmp = realloc(*pre_string, sizeof(char) * (*len * 3));
    if (tmp == NULL) {
        free(pre_string);
        *pre_string = NULL;
        return 0;
    }
    *pre_string = tmp;
    return 1;
}

/* sets the string for the previous element in the prestring */
static void set_prev_pre_string(int depth, char *pre_string) {
    if (strncmp("`--", pre_string + (depth - 1) * 3, 3) == 0)
        strncpy(pre_string + (depth - 1) * 3, "   ", 3);
    else
        strncpy(pre_string + (depth - 1) * 3, "|  ", 3);
}

/* sets the new element in the pre string */
static void set_next_pre_string(int last, int depth, char *pre_string) {
    if (last)
        strncpy(pre_string + depth * 3, "`--", 3);
    else
        strncpy(pre_string + depth * 3, "+--", 3);
}

/* prints the given file and all files below it */
void print_files(file_s *file) {
    static int depth = 0, len = 1;
    static char *pre_string = NULL;
    if (depth == -1) {
        free(pre_string);
        return;
    }

    if (pre_string == NULL) {
        pre_string = malloc(sizeof(char) * (len * 3));
        if (pre_string == NULL) {
            depth = -1;
            return;
        }
    }

    if (depth >= len)
        if (resize_pre_string(&len, &pre_string) == 0)
            depth = -1;

    print_file_search(file, pre_string, depth * 3);

    if (depth > 0)
        set_prev_pre_string(depth, pre_string);

    for (int i = 0; i < file->num_files; i++) {
        set_next_pre_string(i == file->num_files - 1, depth, pre_string);
        depth++;
        print_files(file->files[i]);
    }

    depth--;

    if (depth == -1)
        free(pre_string);
}

/* recursive search for files to create the tree */
static void search_r(file_s *file) {
    if (file->type == TYPE_DIR)
        if (open_dir(file))
            for (int i = 0; i < file->num_files; i++)
                search_r(file->files[i]);
}

/* calculates stats for the given file struct tree */
static void calculate_file_stats(file_s *file) {
    for (int i = 0; i < file->num_files; i++) {
        calculate_file_stats(file->files[i]);

        file->total_size += file->files[i]->total_size;
        file->total_num_files += file->files[i]->total_num_files;
    }
}

/* compare for sorting files */
static int file_s_cmp(const void *file_1, const void *file_2) {
    return (*(file_s**)file_2)->total_size - (*(file_s**)file_1)->total_size;
}

/* sort file function recursively sorts the entire tree */
static void sort_files(file_s *file) {
    if (file == NULL)
        return;

    qsort(file->files, file->num_files, sizeof(file_s*), file_s_cmp);

    for (int i = 0; i < file->num_files; i++)
        if (file->files[i]->num_files > 0)
            sort_files(file->files[i]);
}

/* searches recursively through the filesystem to create a file structure tree */
file_s *search(char *start_filename) {
    if (start_filename == NULL)
        return NULL;

    file_s *file = get_info(start_filename);
    if (file == NULL)
        return NULL;

    search_r(file);

    calculate_file_stats(file);
    sort_files(file);

    return file;
}