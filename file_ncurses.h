/* Author: Aidan
 * Date: Aug, 20, 17
 * File: file_ncurses
 * Description:
 */
#ifndef FILE_NCURSES_H
#define FILE_NCURSES_H

#include <ncurses.h>

#include "file_info.h"

void init_ncurses();

void display_view(file_struct *start);

void exit_ncurses();

#endif /* FILE_NCURSES_H */
