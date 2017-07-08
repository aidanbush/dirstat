/* Author: Aidan
 * Date: June, 8, 17
 * File:
 * Description:
 */
#include <dirent.h>
#include <stdio.h>

#include "file_info.h"
#include "open_directory.h"

void print_dirent();

void open_dir(file_struct* file){
    DIR* dir_struct = NULL;
    struct dirent* file_dirent = NULL;

    dir_struct = opendir(file->name);
    if (dir_struct == NULL) {
        return;
    }

    while((file_dirent = readdir(dir_struct)) != NULL){
        print_dirent(file_dirent);
    }
    closedir(dir_struct);// check output

    //free structs
    free(file_dirent);
    return;
}

void print_dirent(struct dirent* file_dirent) {
    printf("printing dirent\n");
    printf("d_ino: %d\n", (int) file_dirent->d_ino);
    printf("d_name: %s\n", file_dirent->d_name);
    return;
}
