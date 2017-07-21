/*
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

void debug_print_files(file_struct* file, int depth, int last) {
    for (int i = 0; i < depth -1; i++)
        printf("|  ");
    if (depth != 0){
        if (last)
            printf("`--");
        else
            printf("+--");
    }
    printf("%s\n", file->name);
    for (int i = 0; i < file->num_files; i++)
        debug_print_files(file->files[i], depth + 1,
                          (i == file->num_files - 1)? 1 : 0);
}

void delete_files(file_struct* file) {
    if (file->type == TYPE_DIR)
        for (int i = 0; i < file->num_files; i++)
            delete_files(file->files[i]);
    free(file->files);
    free(file->name);
    free(file->path);
    free(file);
}

/*
void generate_calculations() {
    return;
}
*/
