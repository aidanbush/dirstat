/* Author: Aidan
 * Date: June, 8, 17
 * File: File Info
 * Description: deals with file_info struct
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>

#include "file_info.h"

//prototypes
int set_struct_file_type();
int resize_struct_files();

file_struct* get_file(char* filename) {
    struct stat* file_stat = NULL;
    file_struct* file_info = NULL;
    int stat_val = 0;

    file_stat = malloc(sizeof(struct stat));
    if (file_stat == NULL) {
        return NULL;
    }

    file_info = malloc(sizeof(file_struct));
    if (file_info == NULL) {
        free(file_stat);
        return NULL;
    }

    stat_val = stat(filename, file_stat);
    fprintf(stderr, "stat_val %d\n", stat_val);
    
    if (stat_val != 0) {
        //print errno
        fprintf(stderr, "errno: %d\n", errno);
        perror("stat error:");
        free(file_stat);
        free(file_info);
        return NULL;
    }

    //set type
    if (set_struct_file_type(file_info, file_stat) == -1) {
        free(file_stat);
        free(file_info);
        fprintf(stderr, "error in getting file type\n");
        return NULL;
    }
    file_info->size = file_stat->st_size;

    file_info->name = strdup(filename);
    if (file_info->name == NULL) {
        free(file_stat);
        free(file_info);
        fprintf(stderr, "error in copying filename\n");
        return NULL;
    }

    file_info->num_files = 0;
    file_info->files = NULL;
    //add file
    return file_info;
}


int set_struct_file_type(file_struct* file_info, struct stat* file_stat) {
    printf("%d\n", file_stat->st_mode);
    file_info->file_mode = file_stat->st_mode;
    switch (file_stat->st_mode) {
        case S_IFMT:
        case S_IFBLK:
        case S_IFCHR:
        case S_IFIFO:
        case S_IFREG:
            file_info->type = TYPE_FILE;
            break;
        case S_IFDIR:
            file_info->type = TYPE_DIR;
            break;
        case S_IFLNK:
            file_info->type = TYPE_LINK;
            break;
        default:
            file_info->type = TYPE_UNKNOWN;
            break;
    }
    return 1;
}


void debug_print_file_s(file_struct* file_s) {
    fprintf(stderr, "filename: %s\n", file_s->name);
    fprintf(stderr, "type: ");
    switch (file_s->type) {
        case TYPE_FILE:
            fprintf(stderr, "TYPE_FILE\n");
            break;
        case TYPE_LINK:
            fprintf(stderr, "TYPE_LINK\n");
            break;
        case TYPE_DIR:
            fprintf(stderr, "TYPE_DIR\n");
            break;
        case TYPE_UNKNOWN:
            fprintf(stderr, "TYPE_UNKNOWN\n");
            break;
        default:
            fprintf(stderr, "ERROR\n");
            break;
    }
    fprintf(stderr, "size: %d\n", (int) file_s->size);
    fprintf(stderr, "number of files: %d\n", file_s->num_files);
}


void add_file_list(file_struct* file, char * new_filename) {
    fprintf(stderr, "add_file_list start\n");
    //create struct
    file_struct* new_file = get_file(new_filename);
    if(new_file == NULL) {
        return;
    }
    //check for space
    if(file->num_files >= file->max_files) {
        if(resize_struct_files(file) == 0) {
            free(new_file);
            return;
        }
    }
    file->files[file->num_files] = new_file;//add struct
    file->num_files += 1;//update num_files
    return;//return
}

int resize_struct_files(file_struct* file) {
    //create new array
    file_struct** new_file_array = malloc(sizeof(file_struct*) * file->max_files * 2);
    if(new_file_array == NULL) {
        return 0;
    }
    //copy elements over
    for(int i = 0; i < file->num_files; i++) {
        new_file_array[i] = file->files[i];
    }
    //free old
    free(file->files);
    //set on struct
    file->files = new_file_array;
    return 1;
}