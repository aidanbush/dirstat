/* Author: Aidan
 * Date: Dec. 18, 17
 * File: open_dirs.h
 * Description: header file for opening the directory of the given file
 */
#ifndef OPEN_DIRS_H
#define OPEN_DIRS_H

#include "file_info.h"

/* opens the directory of the given file struct, and adds all files within it */
int open_dir(file_s *file);

#endif /* OPEN_DIRS_H */