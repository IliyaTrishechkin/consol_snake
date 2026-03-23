#include <ncurses.h>
#include "list_lib.h" // I used list for snake
#include <stdlib.h>   // for random number to point
#include <time.h>     // for random number to point
#include <unistd.h>
#include <string.h>
//#include <stdio.h>


enum {time_wait = 100};
enum {key_escape = 27};
enum {color_count = 8};

static const int all_colors[color_count] = {
    COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
    COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
};

static const char win_game[] = "You won, but the next update will be coming soon.";
static const char lose_game[] = "You lost. Try again.";

const char snake_char = '*';
const char point_char = '$';


// save all information about window object
struct Window{
    int row; // Oy or Y
    int low; // Ox or X
    int count_point; // count point
    bool color; // if terminal can use colors
};

struct point{
    int point_x, point_y; // coord apple
};


// generate point(apple)
void set_point(struct point* point, struct Window* window, struct List* snake){
    int total = window->low * window->row;
    int *occupied = calloc(total, sizeof(int));               // make array
    while(snake){                                              // delete all occupied characters
        int idx = snake->data.y * window->low + snake->data.x;
        occupied[idx] = 1;
        snake = snake->next;
    }

    int free_count = 0;                                       // count free characters
    for (int i = 0; i < total; i++) {
        if (!occupied[i]) free_count++;
    }

    int target = rand() % free_count;                         // generate

    for (int i = 0; i < total; i++) {                         // make coords
        if (!occupied[i]) {
            if (target == 0) {
                point->point_x = i % window->low;
                point->point_y = i / window->low;
                break;
            }
            target--;
        }
    }
    free(occupied);
}


void move_snake(enum direction direction, struct Window* window, struct List* snake){
    type data = snake->data, prw_data;
    switch(direction){
        case up:    snake->data.y--; break;
        case down:  snake->data.y++; break;
        case left:  snake->data.x--; break;
        case right: snake->data.x++; break;
    }
    if(snake->data.y < 0) snake->data.y = window->row - 1;      // checking for going beyond the boundaries
    snake->data.y = snake->data.y % (window->row);              // checking for going beyond the boundaries
    if(snake->data.x < 0) snake->data.x = window->low - 1;      // checking for going beyond the boundaries
    snake->data.x = snake->data.x % (window->low);              // checking for going beyond the boundaries
    snake = snake->next;
    while(snake){
        prw_data = snake->data;
        snake->data = data;
        data = prw_data;
        snake = snake->next;
    }
}


void point_check(struct point* point, struct Window* window, struct List* snake, enum direction direction){
    if(snake->data.x == point->point_x && snake->data.y == point->point_y){
        int len = len_list(snake);
        if(len == 1){
            switch(direction){
                case up:    push_back(snake, (type){point->point_x, point->point_y+1}); break;
                case down:  push_back(snake, (type){point->point_x, point->point_y-1}); break;
                case left:  push_back(snake, (type){point->point_x+1, point->point_y}); break;
                case right: push_back(snake, (type){point->point_x-1, point->point_y}); break;
            }
            window->count_point += 1;                                       // add point
            set_point(point, window, snake);                                // new point
            return ;
        }
        struct List* tail = get_last(snake);
        struct List* second_tail = iteration(snake, len-1);
        push_back(snake, (type){tail->data.x - (second_tail->data.x - tail->data.x), tail->data.y - (second_tail->data.y - tail->data.y)});
        window->count_point += 1;                                           // add point
        set_point(point, window, snake);                                    // new point
    }
    return ;
}

void game_over(bool flag, struct Window* window){
    if(flag){
        int x = (window->low - strlen(win_game))/2;
        int y = (window->row - 1)/2;
        if(window->color){attrset(COLOR_PAIR(4));}
        mvprintw(y, x, "%s", win_game);
    } else {
        int x = (window->low - strlen(lose_game))/2;
        int y = (window->row - 1)/2;
        if(window->color){attrset(COLOR_PAIR(4));}
        mvprintw(y, x, "%s", lose_game);
    }
    mvprintw(window->row/2+1, window->low/2, "total: %d", window->count_point);
    refresh();
    return ;
}

// checks all barriers or body
bool check_snake(struct Window* window, struct List* snake){
    if(len_list(snake) >= window->low * window->row){
        game_over(true, window);
        return false;
    }
    struct List* snake_head = snake;
    snake = snake->next;
    while(snake){
        if(snake_head->data.x == snake->data.x && snake_head->data.y == snake->data.y){
            game_over(false, window);
            return false;
        }
        snake = snake->next;
    }
    return true;
}


void write_to_display(struct point* point, struct Window* window, struct List* snake){
    clear(); 
    if(window->color){attrset(COLOR_PAIR(3));}
    mvprintw(0, 0, "%d", window->count_point);
    if(window->color){attrset(COLOR_PAIR(1));}
    while(snake){
        move(snake->data.y, snake->data.x);
        addch(snake_char);
        snake = snake->next;
    }
    if(window->color){attrset(COLOR_PAIR(2));}
    move(point->point_y, point->point_x);
    addch(point_char);
    refresh();
}



int main(int argv, char** argc){
    srand(time(NULL));
    struct Window main_window;
    main_window.count_point = 0;  
    struct point point; 
    int key; 
    initscr();                    // init
    cbreak();                     // disables input buffering
    timeout(time_wait);           // press wait time
    keypad(stdscr, 1);            // setting keyboard
    noecho();                     // disables show input
    curs_set(0);                  // delete cursor
    main_window.color = has_colors();
    if(main_window.color){
        start_color();
        // 1 number pair, 2 color text, 3 color background
        init_pair(1, all_colors[3], all_colors[6]); // snake
        init_pair(2, all_colors[1], all_colors[0]); // point
        init_pair(3, all_colors[2], all_colors[0]); // text progress
        init_pair(4, all_colors[1], all_colors[4]); // End or win game
    }
    getmaxyx(stdscr, main_window.row, main_window.low);
    struct List* snake = initialize((type){main_window.low/2, main_window.row/2, up});
    enum direction direction =  up;
    set_point(&point, &main_window, snake);switch(snake->data.direction){
        case up:    snake->data.y--; break;
        case down:  snake->data.y++; break;
        case left:  snake->data.x--; break;
        case right: snake->data.x++; break;
    }
    while((key = getch()) != key_escape){
        switch(key){
        case KEY_UP:
            direction = up;
            break;
        case KEY_DOWN:
            direction = down;
            break;
        case KEY_LEFT:
            direction = left;
            break;
        case KEY_RIGHT:
            direction = right;
            break;
        case ERR:
            break;
        case KEY_RESIZE:
            getmaxyx(stdscr, main_window.row, main_window.low);
            set_point(&point, &main_window, snake);
            break;
        }
        move_snake(direction, &main_window, snake);
        point_check(&point, &main_window, snake, direction);
        if(!check_snake(&main_window, snake)){
            sleep(5);
            break;
        }
        write_to_display(&point, &main_window, snake);
    }

    endwin();                     // return control
    return 0;
}
