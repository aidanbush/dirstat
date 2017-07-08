/* Author: Aidan
 * Date: June, 8, 17
 * File: File Info
 * Description:
 */

#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

//structs
typedef enum {
    TYPE_FILE = 0,
    TYPE_LINK,
    TYPE_DIR,
    TYPE_UNKNOWN
} file_type;

typedef struct file_struct {
    off_t size;
    file_type type;
    int file_mode;
    int num_files;
    char* name;
    struct file_struct** files;
} file_struct;

//functions
file_struct* get_file(char*);
void debug_print_file_s(file_struct*);

#endif
