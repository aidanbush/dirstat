/* Author: Aidan
 * Date: Dec. 19, 17
 * File: search.h
 * Description: header file for recursively searching though directories.
 */
#ifndef SEARCH_H
#define SEARCH_H

#include "file_info.h"

/* searches though and creates a tree of file structures for all the sub files
 * and directories from the given file name. */
file_s *search(char *start_filename);

/* prints out the given file struct and all the files below it in the tree */
void print_files(file_s *file);

#endif /* SEARCH_H */