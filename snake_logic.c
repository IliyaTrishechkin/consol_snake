#include <ncurses.h>
#include <stdlib.h>         // for random number to point
#include <time.h>           // for random number to point
#include "list_lib.h"       // I used list for snake
#include "snake_logic.h"



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

// checks all barriers or body
enum status_snake check_snake(struct Window* window, struct List* snake){
    if(len_list(snake) >= window->low * window->row){
        return snake_win;
    }
    struct List* snake_head = snake;
    snake = snake->next;
    while(snake){
        if(snake_head->data.x == snake->data.x && snake_head->data.y == snake->data.y){
            return snake_lose;
        }
        snake = snake->next;
    }
    return snake_none;
}