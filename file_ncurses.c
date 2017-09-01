/* Author: Aidan
 * Date: Aug, 16, 2017
 * File: file_ncurses.c
 * Description:
 */

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "file_ncurses.h"
#include "file_info.h"

#define TOP_HEIGHT 2

#define DET_HEIGHT 4
#define DET_START (LINES) - (DET_HEIGHT)

#define MAIN_HEIGHT (LINES) - (TOP_HEIGHT) - (DET_HEIGHT)
#define MAIN_START (TOP_HEIGHT)

#define POST_LEN 7


typedef struct print_array {
    int num;
    int size;
    file_struct **f_arr;
} print_array;


char* convert_file_size(off_t size) {
    static char *postfix[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
    int i = 0;
    char *new_size = NULL;

    while(size > 1024 && i < POST_LEN) {
        size /= 1024;
        i++;
    }
    if (sprintf(new_size, "%ld%s", size, postfix[i]) < 0)
        new_size = strcpy(new_size, "ERROR");

    return new_size;
}

void init_ncurses() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    refresh();
}

WINDOW* create_top_view() {
    WINDOW* top;

    //create window along the top with a height of 1
    top = newwin(TOP_HEIGHT, COLS, 0, 0);

    //add menu info
    mvwprintw(top, 0, 0, "name");
    mvwprintw(top, 0, 40, "size");
    mvwprintw(top, 0, 50, "t. size");
    mvwprintw(top, 0, 60, "type");
    mvwprintw(top, 0, 70, "t. files");

    //add bottom border
    mvwhline(top, 1, 0, '-', COLS);

    wrefresh(top);
    return top;
}

void destroy_top_view(WINDOW* top_v) {
    delwin(top_v);
}

char *get_main_line(file_struct *printing) {
    static char *str_arr[] = {
"dir1                                    1024      2048      DIR       1",
" file1                                  1024      1024      FILE      0",
"dir2                                    1024      55607     DIR       4",
" file2                                  7685      7685      FILE      1",
" dir3                                   1024      46898     DIR       1",
"  file3                                 45874     45874     FILE      1",
"file4                                   485       485       FILE      1",
"file5                                   5682      5682      DIR       1",
"dir3                                    1024      16810     DIR       3",
" file6                                  2546      2546      FILE      1",
" file7                                  13240     13240     FILE      1",
"dir4                                    1024      125888    DIR       8",
" dir5                                   1024      116952    DIR       5",
"  dir6                                  1024      51404     DIR       3",
"   file8                                45856     45856     FILE      1",
"   file9                                4524      4524      FILE      1",
"  file10                                64524     64524     FILE      1",
" file11                                 4658      4658      FILE      1",
" file12                                 3254      3254      FILE      1",
"file13                                  580       580       FILE      1",
};
    static int i = 0;
    char *new_str = malloc(sizeof(char) * 80);

    new_str = strcpy(new_str, str_arr[i]);

    i = (i + 1) % 20;

    return new_str;
}


void add_p_array(print_array* array, file_struct *cur) {
    if (array->num >= array->size)
        return;
    array->f_arr[array->num] = cur;
    array->num++;
}

file_struct *get_next_above(file_struct *cur) {
    file_struct *next = cur->parent;
    if (next == NULL)
        return NULL;

    int i = 0;

    while (next->files[i] != cur && i < next->num_files)
        i++;

    //get files after cur
    i++;
    if (i >= next->num_files)
        return get_next_above(next);
    return next->files[i];
}

void get_print_array(file_struct *start, print_array *array) {
    file_struct *cur = start;
    add_p_array(array, cur);
    bool done = false;
    file_struct *next = NULL;

    while(array->num <= array->size && done != true) {
        next = NULL;
        if (cur->min != true)
            // incase of error with the tree
            for (int i = 0; i < cur->num_files; i++)
                if (cur->files[i] != NULL) {
                    next = cur->files[i];
                    break;
                }

        if (next == NULL) // no children
            next = get_next_above(cur);

        if (next == NULL){
            done = true;
            continue;
        }
        add_p_array(array, next);
        cur = next;
    }
}

/*
file_struct *next_print_file(file_struct *last_print) {
    //check for children
    if (last_print->min == false && last_print->num_files > 0)
        for (int i = 0; i < last_print->num_files; i++)
            return last_print->files[i];

    //check for siblings
    file_struct *parent = last_print->parent;
    if (parent != NULL)
        if (parent->num_files > 0) {
            int i = 0;
            // find last_print
            while (i < parent->num_files && parent->files[i] != last_print)
                i++;
            // increment to next files after last_print
            i++;
            // if the sibling exists
            if (i < parent->num_files)
                return parent->files[i];
        }

    //go to parents
}
*/

void print_files_w(print_array *array, WINDOW *main_v) {
    //
    char *line_str = NULL;
    int i = 0;
    while (i < array->num) {
        //create string
        line_str = calloc(80, sizeof(char));
        sprintf(line_str, "%s", array->f_arr[i]->name);
        mvwprintw(main_v, i, 0, "%s", line_str);
        free(line_str);
        i++;
    }
}

WINDOW* create_main_view(file_struct *start) { 
    WINDOW *main_v;
    //char *line_str = NULL;
    //file_struct *cur_print = start;

    //get array for print
    print_array *p_arr = malloc(sizeof(print_array));
    p_arr->size = MAIN_START;
    p_arr->num = 0;
    file_struct **arr_files = malloc(sizeof(file_struct*) * p_arr->size);
    p_arr->f_arr = arr_files;

    get_print_array(start, p_arr);

    //create window between the top and bottom views
    main_v = newwin(MAIN_HEIGHT, COLS, MAIN_START, 0);

    //fprintf(stderr, "printing arr\n");
    print_files_w(p_arr, main_v);

    /*
    for (int i =0; i < MAIN_HEIGHT; i++) {
        line_str = get_main_line(cur_print);
        mvwprintw(main_v, i, 0, "%s", line_str);
        free(line_str);
        cur_print = next_print_file(cur_print);
    }
    */

    free(p_arr->f_arr);
    free(p_arr);

    wrefresh(main_v);
    return main_v;
}

void destory_main_view(WINDOW* main_v) { 
    delwin(main_v);
}

WINDOW* create_detail_view() { 
    WINDOW* det;

    //create window along bottom
    det = newwin(DET_HEIGHT, COLS, DET_START, 0);

    //add top border
    whline(det, '-', COLS);

    //add info
    mvwprintw(det, 1, 0, "name:");
    mvwprintw(det, 2, 0, "path:");
    //last line
    mvwprintw(det, 1, 40, "size:");
    mvwprintw(det, 3, 0, "t. size:");
    mvwprintw(det, 3, 20, "files:");
    mvwprintw(det, 3, 40, "t. files:");
    mvwprintw(det, 3, 60, "type:");

    wrefresh(det);
    return det;
}

void destroy_det_view(WINDOW* det_v) { 
    delwin(det_v);
}

void display_view(file_struct *start) { 
    WINDOW *top_v, *main_v, *detail_v;
    int c;
    bool exit = false;
    //int pos = 0;
    //display top view
    top_v = create_top_view();
    //display bottom view
    detail_v = create_detail_view();
    //display main view
    main_v = create_main_view(start);

    //get input
    while(exit == false) { 
        c = getch();
        switch(c) {
            case 'q':
                exit = true;
                continue;
                break;
            case KEY_UP:
                //move_up();
                break;
            case KEY_DOWN:
                //move_down();
                break;
            case KEY_RIGHT:
                //move_in();
                break;
            case KEY_LEFT:
                //move_out();
                break;
        }
        //update top
        //update main
        //update bottom
    } 
    destroy_top_view(top_v);
    destory_main_view(main_v);
    destroy_det_view(detail_v);
} 

void exit_ncurses() { 
    endwin();
} 


