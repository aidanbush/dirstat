/* Author: Aidan
 * Date: Dec. 18, 17
 * File: file_info.h
 * Description: header file for getting the info for given files
 */
#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <sys/types.h>

typedef struct file_s {
    off_t size;
    char *name;
    char *path;
} file_s;

void free_file_s(file_s *file);

file_s *get_info(char *pathname);

void print_file_s(file_s *file);

#endif /* FILE_INFO_H */
