/* Author: Aidan
 * Date: July, 16, 17
 * File: File Search
 * Description: deals with recursivly searching through filesystem
 */

#include <stdio.h>

#include "file_info.h"
#include "open_directory.h"
#include "file_search.h"

void search(file_struct* file) {
    if (file->type == TYPE_DIR)
        if (open_dir(file))
            for (int i = 0; i < file->num_files; i++)
                search(file->files[i]);
}

void debug_print_files(file_struct* file) {
    printf("%s\n", file->name);
    for (int i = 0; i < file->num_files; i++)
        debug_print_files(file->files[i]);
}

void delete_files(file_struct* file) {
    if (file->type == TYPE_DIR)
        for (int i = 0; i < file->num_files; i++)
            delete_files(file->files[i]);
    free(file->files);
    free(file->name);
    free(file);
}
