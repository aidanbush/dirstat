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
char* create_filename();
int ignored_filename();

int open_dir(file_struct* file){
    DIR* dir_struct = NULL;
    struct dirent* file_dirent = NULL;
    char* filename = NULL;

    dir_struct = opendir(file->name);
    if (dir_struct == NULL) {
        return 0;
    }

    while((file_dirent = readdir(dir_struct)) != NULL){
        //if in list of ignored files
        if (ignored_filename(file_dirent->d_name))
            continue;
        //create proper filename
        filename = create_filename(file->name, file_dirent->d_name);
        //add filename to file struct
        add_file_list(file, filename);
        //print_dirent(file_dirent);
        free(filename);
    }
    closedir(dir_struct);// check output

    //free structs
    free(file_dirent);
    return 1;
}

void print_dirent(struct dirent* file_dirent) {
    printf("printing dirent\n");
    printf("d_ino: %d\n", (int) file_dirent->d_ino);
    printf("d_name: %s\n", file_dirent->d_name);
    return;
}

char* create_filename(char* name_start, char* name_end) {
    char* filename = malloc(sizeof(char) *
                           (strlen(name_start) + strlen(name_end) + 2));
    
    if (filename == NULL) {
        return NULL;
    }

    sprintf(filename, "%s/%s", name_start, name_end);

    return filename;
}

int ignored_filename(char* filename) {
    if(strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0)
        return 1;
    return 0;
}
