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


void add_p_array(print_array* array, file_struct *cur) {
    if (array->num >= array->size || cur == NULL)
        return;
    array->f_arr[array->num] = cur;
    array->num++;
    //fprintf(stderr, "add array f:%s\n", cur->name);
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

    while(array->num < array->size && done != true) {
        next = NULL;
        // check children
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


// prints array of files
void print_files_w(print_array *array, WINDOW *main_v) {
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

    //get array for print
    print_array *p_arr = malloc(sizeof(print_array));
    p_arr->size = MAIN_HEIGHT;
    p_arr->num = 0;
    file_struct **arr_files = malloc(sizeof(file_struct*) * p_arr->size);
    p_arr->f_arr = arr_files;

    get_print_array(start, p_arr);

    //create window between the top and bottom views
    main_v = newwin(MAIN_HEIGHT, COLS, MAIN_START, 0);

    print_files_w(p_arr, main_v);

    free(p_arr->f_arr);
    free(p_arr);

    wrefresh(main_v);
    return main_v;
}


void destory_main_view(WINDOW* main_v) { 
    delwin(main_v);
}


WINDOW* create_detail_view(file_struct *cur) { 
    WINDOW* det;

    //create window along bottom
    det = newwin(DET_HEIGHT, COLS, DET_START, 0);

    //add top border
    whline(det, '-', COLS);

    //add info
    mvwprintw(det, 1, 0, "name: %s", get_file_name(cur));
    mvwprintw(det, 1, 40, "size: %ld", get_file_size(cur));
    mvwprintw(det, 2, 0, "path: %s", get_file_path(cur));
    //last line
    mvwprintw(det, 3, 0, "t. size: %ld", get_file_t_size(cur));
    mvwprintw(det, 3, 20, "files: %d", cur->num_files);
    mvwprintw(det, 3, 40, "t. files: %d", cur->total_num_files);
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
    detail_v = create_detail_view(start);
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


