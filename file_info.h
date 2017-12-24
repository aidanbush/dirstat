/* Author: Aidan
 * Date: Dec. 18, 17
 * File: file_info.h
 * Description: header file for getting the info for given files
 */
#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <sys/types.h>

typedef enum {
    TYPE_REG = 0,
    TYPE_LINK,
    TYPE_DIR,
    TYPE_UNKNOWN,
} file_type;

typedef struct file_s {
    off_t size;
    off_t total_size;
    char *name;
    char *path;
    file_type type;
    int num_files;
    int max_files;
    int total_num_files;
    struct file_s **files;
    struct file_s *parent;
} file_s;

/* frees the given file_s struct */
void free_file_s(file_s *file);

/* returns a file_s struct for the given pathname */
file_s *get_info(char *pathname);

/* prints the given file_s struct */
void print_file_s(file_s *file);

/* adds a file given a pathname */
int add_file(file_s *parent, char *pathname);

#endif /* FILE_INFO_H */