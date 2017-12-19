/* Author: Aidan
 * Date: Dec. 18, 17
 * File: file_info.c
 * Description: main file for getting the info of a file
 */

/* stardard library includes*/
#include <string.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

#include <sys/stat.h>

/* project includes */
#include "file_info.h"

/* frees the file_s struct */
void free_file_s(file_s *file) {
    if (file == NULL)
        return;

    if (file->name != NULL) {
        free(file->name);
        file->name = NULL;
    }

    if (file->path != NULL) {
        free(file->path);
        file->path = NULL;
    }

    //free children
    for (int i = 0; i < file->num_files; i++)
        free_file_s(file->files[i]);
    free(file->files);

    free(file);
}

static void add_file_type(file_s *file, struct stat file_stat) {
    if (S_ISREG(file_stat.st_mode)) {
        file->type = TYPE_REG;
    } else if (S_ISLNK(file_stat.st_mode)) {
        file->type = TYPE_LINK;
    } else if (S_ISDIR(file_stat.st_mode)) {
        file->type = TYPE_DIR;
    } else {
        file->type = TYPE_UNKNOWN;
    }
}

static int set_files(file_s *file) {
    file->num_files = 0;
    file->max_files = 0;
    file->files = NULL;
    return 1;
}

/* creates a file_s struct for the given pathname */
file_s *get_info(char *pathname) {
    struct stat file_stat;
    file_s *file = NULL;

    int err = lstat(pathname, &file_stat);

    if (err != 0) {
        perror("stat");
        return NULL;
    }

    file = malloc(sizeof(file_s));
    if (file == NULL) {
        return NULL;
    }

    file->path = strndup(pathname, PATH_MAX);
    if (file->path == NULL) {
        free_file_s(file);
        return NULL;
    }

    file->name = strndup(basename(pathname), NAME_MAX);
    if (file->name == NULL) {
        free_file_s(file);
        return NULL;
    }

    if (set_files(file) == 0) {
        free_file_s(file);
        return NULL;
    }


    add_file_type(file, file_stat);
    file->size = file_stat.st_size;
    return file;
}

/* prints a file_s struct */
void print_file_s(file_s *file) {
    printf("filename: %s\n"
        "pathname: %s\n"
        "size: %lu\n",
        file->name, file->path, file->size);
}