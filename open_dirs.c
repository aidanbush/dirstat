/* Author: Aidan
 * Date: Dec. 18, 17
 * File: open_dirs.c
 * Description: file for opening the directory of a given file_s struct and
 *  adding all its files.
 */

/* standard library includes*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <dirent.h>

/* project includes */
#include "open_dirs.h"
#include "file_info.h"

#define FULL_PATH   "%s/%s"

/* creates full pathname by concatenating the path, and file name */
static char *create_full_path(char *path, char *name) {
    int size = snprintf(NULL, 0, FULL_PATH, path, name) + 1;
    if (size >= PATH_MAX)
        return NULL;

    char *full_path = calloc(size, sizeof(char));
    if (full_path == NULL)
        return NULL;

    snprintf(full_path, size, FULL_PATH, path, name);

    return full_path;
}

/* given a file and path name returns 1 if they are to be ignored */
static int ignore_file(char *name, char *path) {
    if (name == NULL || path == NULL)
        return 1;

    if (strncmp(name, ".", NAME_MAX) == 0 || strncmp(name, "..", NAME_MAX) == 0
        || strncmp(path, "//proc", PATH_MAX) == 0)
        return 1;

    return 0;
}

/* opens the directory of the given file struct, and adds all files within it */
int open_dir(file_s *file) {
    if (file->type != TYPE_DIR)
        return 0;

    DIR* dir = NULL;
    struct dirent* file_dirent = NULL;
    char *filename;

    dir = opendir(file->path);
    if (dir == NULL) {
        return 0;
    }

    while((file_dirent = readdir(dir)) != NULL) {
        filename = create_full_path(file->path, file_dirent->d_name);

        if (ignore_file(file_dirent->d_name, filename)) {
            free(filename);
            continue;
        }

        add_file(file, filename);
        free(filename);
    }

    closedir(dir);
    return 1;
}