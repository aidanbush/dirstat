/* Author: Aidan
 * Date: July, 16, 17
 * File: File Search
 * Description: deals with recursivly searching through filesystem
 */

#include <stdio.h>
#include <stdlib.h>

#include "file_info.h"
#include "open_directory.h"
#include "file_search.h"


int resize_pre_string(pre_string*);

void search(file_struct* file) {
    if (file->type == TYPE_DIR)
        if (open_dir(file))
            for (int i = 0; i < file->num_files; i++)
                search(file->files[i]);
}

void debug_print_files(file_struct* file, pre_string* str) {
    if (str->depth >= str->len)
        resize_pre_string(str);

    for (int i = 0; i < str->depth; i++)
        printf("%s", str->str[i]);

    if (str->depth > 0){
        if (strcmp("`--", str->str[str->depth -1]) == 0)//check if last
            strcpy(str->str[str->depth -1], "   ");
        else
            strcpy(str->str[str->depth -1], "|  ");
    }

    printf("%s\n", file->name);
    for (int i = 0; i < file->num_files; i++){
        if (i < file->num_files -1)
            strcpy(str->str[str->depth], "+--");
        else
            strcpy(str->str[str->depth], "`--");
        str->depth++;
        debug_print_files(file->files[i], str);
    }
    str->depth--;
}

void delete_files(file_struct* file) {
    if (file->type == TYPE_DIR)
        for (int i = 0; i < file->num_files; i++)
            delete_files(file->files[i]);

    if (file->files != NULL)
        free(file->files);
    free(file->name);
    free(file->path);
    free(file);
}

pre_string* create_pre_string(int len) {
    pre_string* str = malloc(sizeof(pre_string));
    if (str == NULL) {      
        fprintf(stderr, "str == NULL\n");     
        return NULL;
    }

    str->str = malloc(sizeof(char*) * len);
    if (str->str == NULL) { 
        fprintf(stderr, "str->str == NULL\n"); 
        free(str);
        return NULL;
    }

    for (int i = 0; i < len; i++) { 
        str->str[i] = malloc(sizeof(char) * 4);
        if (str->str[i] == NULL) {
            for (int j = 0; j < i; j++)
                free(str->str[i]);
            free(str->str);
            free(str);
            return NULL;
        }
    }
    str->len = len;
    str->depth = 0;
    return str;
}

//rewrite causes memory errors
int resize_pre_string(pre_string* str) {
    int new_len = str->len * 2;
    //create new array
    char** new_str = malloc(sizeof(char*) * new_len);
    if (new_str == NULL) {
        fprintf(stderr, "new_str == NULL\n");
        return 0;
    }
    //copy over
    for (int i = 0; i < str->depth; i++)
        new_str[i] = str->str[i];

    for (int i = str->depth; i < new_len; i++){
        new_str[i] = malloc(sizeof(char) * 4);
        if (new_str[i] == NULL) {
            fprintf(stderr, "str->str[i] == NULL\n");
            for (int j = str->depth; j < i; j++)
                free(new_str[j]);
            fprintf(stderr, "error in creating new strings for array\n");
            return 0;
        }
    }

    str->len = new_len;
    free(str->str);
    str->str = new_str;
    return 1;
}

void free_pre_string(pre_string* str) {
    for (int i = 0; i < str->len; i++)
        free(str->str[i]);
    free(str->str);
    free(str);
}
/*
void generate_calculations() {
    return;
}
*/
