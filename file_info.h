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
    char *name;
    char *path;
    file_type type;
    int num_files;
    int max_files;
    struct file_s **files;
} file_s;

/* frees the given file_s struct */
void free_file_s(file_s *file);

/* returns a file_s struct for the given pathname */
file_s *get_info(char *pathname);

/* prints the given file_s struct */
void print_file_s(file_s *file);

#endif /* FILE_INFO_H */
