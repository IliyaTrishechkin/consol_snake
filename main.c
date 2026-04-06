#include <ncurses.h>
#include <string.h>
#include "snake_logic.h"


static const char* items[] = {
    "Start",
    "Settings",
    "Exit"
};

enum {number_item = sizeof(items) / sizeof(items[0])};
enum {key_escape = 27};
enum {color_count = 8};

const int all_colors[color_count] = {
    COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
    COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
};


void show_list_options(int height, int wigth, int choce_item){
    for(int i = 0; i < number_item; i++){
        move(((height-number_item)/2 + i), ((wigth - strlen(items[i]))/2));
        if(choce_item == i) attron(A_REVERSE);
        printw("%s", items[i]);
        if(choce_item == i) attroff(A_REVERSE);
    }
}


int main(int argv, char** argc){
    initscr();                      // init
    cbreak();                       // disables input buffering
    keypad(stdscr, 1);              // setting keyboard
    noecho();                       // disables show input
    curs_set(0);                    // delete cursor

    int height, width;
    getmaxyx(stdscr, height, width);
    
    int key;
    int choice_item = 0;            // choce options number

    show_list_options(height, width, choice_item);

    while((key = getch()) != key_escape){
        clear();
        switch (key) {
            case KEY_UP:
                choice_item--;
                if(choice_item < 0) choice_item = number_item - 1;
                break;

            case KEY_DOWN:
                choice_item++;
                if(choice_item > number_item) choice_item = 0;
                break;

            case 10:
            case KEY_ENTER:
                mvprintw(0, 0, "%s", items[choice_item]);
                if(choice_item == 0){
                    main_snake();
                } else if(choice_item == 2){
                    endwin();
                    return 0;
                }
                break;
        }
        show_list_options(height, width, choice_item);
        refresh();
    }

    endwin();
    return 0;
}
