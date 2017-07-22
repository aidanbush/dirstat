/* Author: Aidan
 * Date: July, 16, 17
 * File: File Search
 * Description:
 */

#ifndef FILE_SEARCH_H
#define FILE_SEARCH_H

typedef struct pre_string {
    char** str;
    int depth;
    int len;
} pre_string;

void search(file_struct*);

void debug_print_files(file_struct*, int, pre_string*);

void delete_files(file_struct*);

pre_string* create_pre_string(int);

#endif
