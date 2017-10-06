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

extern int v;

//prototypes
int set_struct_file_type();
int resize_struct_files();

file_struct* get_file(char* filename, char* pathname) {
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

    stat_val = lstat(pathname, file_stat);
    
    if (stat_val != 0) {
        //print errno
        fprintf(stderr, "errno: %d\n", errno);
        perror("stat error");
        fprintf(stderr, "pathname: %s\n", pathname);
        //set type
        //print file
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
        free(file_info->name);
        free(file_info);
        fprintf(stderr, "error in copying filename\n");
        return NULL;
    }
    
    file_info->path = strdup(pathname);
    if (file_info->path == NULL) {
        free(file_stat);
        free(file_info->name);
        free(file_info->path);
        free(file_info);
        fprintf(stderr, "error in copying pathname\n");
        return NULL;
    }

    file_info->num_files = 0;
    file_info->total_num_files = 0;
    file_info->max_files = 0;
    file_info->total_size = file_info->size;
    file_info->files = NULL;
    file_info->parent = NULL;
    file_info->min = false;//true;

    free(file_stat);
    return file_info;
}


int set_struct_file_type(file_struct* file_info, struct stat* file_stat) {
    file_info->file_mode = file_stat->st_mode;

    if (S_ISREG(file_stat->st_mode)) {// reg file
        file_info->type = TYPE_FILE;
    } else if (S_ISLNK(file_stat->st_mode)) {// link
        file_info->type = TYPE_LINK;
    } else if (S_ISDIR(file_stat->st_mode)) {// directory
        file_info->type = TYPE_DIR;
    } else {// other
        file_info->type = TYPE_UNKNOWN;
    }

    return 1;
}


void debug_print_file_s(file_struct* file_s) {
    fprintf(stderr, "filename: %s\n", file_s->name);
    fprintf(stderr, "pathname: %s\n", file_s->path);
    fprintf(stderr, "file mode: %d\n", file_s->file_mode);
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


void add_file_list(file_struct* file, char* new_filename, char* new_pathname) {
    //create struct
    file_struct* new_file = get_file(new_filename, new_pathname);
    if(new_file == NULL) {
        fprintf(stderr, "error in allocating space for current file\n");
        return;
    }
    //check for space
    if(file->num_files >= file->max_files) {
        if(resize_struct_files(file) == 0) {
            fprintf(stderr, "error in allocating space for files\n");
            free(new_file);
            return;
        }
    }
    file->files[file->num_files] = new_file;//add struct
    file->num_files += 1;//update num_files
    file->total_num_files = file->num_files;
    //debug_print_file_s(new_file);
    return;//return
}

int resize_struct_files(file_struct* file) {
    int new_size;

    if(file->max_files == 0)
        new_size = 1;
    else
        new_size = file->max_files * 2;
    
    //create new array
    file_struct** new_file_array = malloc(sizeof(file_struct*) * new_size);

    if(new_file_array == NULL)
        return 0;

    //copy elements over
    for(int i = 0; i < file->num_files; i++)
        new_file_array[i] = file->files[i];

    //free old
    free(file->files);
    //set on struct
    file->files = new_file_array;
    file->max_files = new_size;
    return 1;
}


/* getter functions */

char* get_file_name(file_struct *file) {
    return file->name;
}

char* get_file_path(file_struct *file) {
    return file->path;
}

off_t get_file_size(file_struct *file) {
    return file->size;
}

off_t get_file_t_size(file_struct *file) {
    return file->total_size;
}

int get_file_num_files(file_struct *file) {
    return file->num_files;
}

int get_file_total_num_files(file_struct *file) {
    return file->total_num_files;
}

bool get_file_min(file_struct *file) {
    return file->min;
}

int get_file_depth(file_struct *file) {
    return file->depth;
}

char *get_file_type(file_struct *file) {
    static char *types[] = {"TYPE_FILE", "TYPE_LINK", "TYPE_DIR", "TYPE_UNKOWN",
                           "ERROR"};

    switch(file->type) {
        case TYPE_FILE:
            return types[0];
            break;
        case TYPE_LINK:
            return types[1];
            break;
        case TYPE_DIR:
            return types[2];
            break;
        case TYPE_UNKNOWN:
            return types[3];
            break;
        default:
            return types[4];
            break;
    }
}
