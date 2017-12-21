/* Author: Aidan
 * Date: Dec. 18, 17
 * File: file_info.c
 * Description: main file for getting the info of a file
 */

/* stardard library includes */
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
    file->parent = NULL;
    return file;
}

/* prints a file_s struct */
void print_file_s(file_s *file) {
    printf("filename: %s\n"
        "pathname: %s\n"
        "size: %ld\n",
        file->name, file->path, file->size);
}

/* resized the file list in the file_s */
static int resize_file_list(file_s *file) {
    int new_size = (file->max_files + 1) *2;

    file_s **tmp = realloc(file->files, new_size * sizeof(file_s *));
    if (tmp == NULL)
        return 0;

    file->files = tmp;

    file->max_files = new_size;
    return 1;
}

/* adds a file as a child to the given parent */
int add_file(file_s *parent, char *pathname) {
    if (parent == NULL)
        return 0;

    file_s *child = get_info(pathname);
    if (child == NULL)
        return 0;

    if (parent->max_files <= parent->num_files) {
        if (resize_file_list(parent) == 0) {
            free_file_s(child);
            return 0;
        }
    }

    child->parent = parent;

    parent->files[parent->num_files] = child;
    parent->num_files++;
    return 1;
}