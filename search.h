/* Author: Aidan
 * Date: Dec. 19, 17
 * File: search.h
 * Description: headder file for recursively searching though directories.
 */
#ifndef SEARCH_H
#define SEARCH_H

#include "file_info.h"

file_s *search(char *start_filename);

void print_files(file_s *file);

#endif /* SEARCH_H */
