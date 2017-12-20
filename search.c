/* Author: Aidan
 * Date: Dec. 19, 17
 * File: search.c
 * Description: file for recursively searching though all the directories
 */

/* stardard library includes*/
#include <stdlib.h>

/* project includes */
#include "search.h"
#include "open_dirs.h"
#include "file_info.h"

static void search_r(file_s *file) {
    if (file->type == TYPE_DIR)
        if (open_dir(file))
            for (int i = 0; i < file->num_files; i++)
                search_r(file->files[i]);
}

file_s *search(char *start_filename) {
    if (start_filename == NULL)
        return NULL;

    file_s *file = get_info(start_filename);
    if (file == NULL)
        return NULL;

    search_r(file);

    return file;
}