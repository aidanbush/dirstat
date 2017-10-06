/* Author: Aidan
 * Date: June, 8, 17
 * File: File Info
 * Description:
 */

#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
    off_t size;// aka long int
    off_t total_size;// size of the file and everything below it
    file_type type;
    int file_mode;
    char* name;
    char* path;
    int num_files;
    int total_num_files;//TODO implement
    int max_files;// size of files array
    struct file_struct** files;
    struct file_struct* parent;//TODO test
    bool min;//TODO implement // if minimized for view
    int depth;//TODO implement // the depth from starting dir
} file_struct;

//functions
file_struct* get_file(char*, char*);
void debug_print_file_s(file_struct*);
void add_file_list(file_struct*, char*, char*);

#endif // FILE_INFO_H
