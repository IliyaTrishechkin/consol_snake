#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>         // for random number to point
#include <time.h>           // for random number to point
#include "list_lib.h"       // I used list for snake
#include "snake_logic.h"

enum {time_wait = 100};
enum {key_escape = 27};
enum colors_object {snake_color = 1, point_color = 2, text_color = 3, special_text_color = 4};
enum color_name {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, color_count};


static const char win_game[] = "You won, but the next update will be coming soon.";
static const char lose_game[] = "You lost. Try again.";
static const char error_file[] = "There is a problem with the file. Please delete it.";
static const char file_name[] = "setting.txt";


static const int all_colors[color_count] = {
    COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
    COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
};

static const char* items[] = {
    "Start", 
    "Settings", 
    "Exit"
};
enum {number_item = sizeof(items) / sizeof(items[0])};


struct setting_char{
    char snake_char;
    char point_char;
};


struct user{
    int max_count_point;
    int games_played;
};

struct color{
    enum color_name snake_fg; enum color_name snake_bg;
    enum color_name point_fg; enum color_name point_bg;
    enum color_name text_fg; enum color_name text_bg;
    enum color_name special_text_fg; enum color_name special_text_bg;
};

// set base settings
void default_config(struct setting_char* setting_char, struct user* user, struct color* color){
    setting_char->snake_char = '*';
    setting_char->point_char = '$';
    user->games_played = 0;
    user->max_count_point = 0;
    color->snake_fg = YELLOW;
    color->snake_bg = CYAN;
    color->point_fg = RED;
    color->point_bg = BLACK;
    color->text_fg = GREEN;
    color->text_bg = BLACK;
    color->special_text_fg = RED;
    color->special_text_bg = BLUE;

    return ;
}


void save_setting_file(const char* filename, struct setting_char* setting_char, struct user* user, struct color* color){
    FILE* file = fopen(filename, "w");
    fprintf(file, "%d %d\n", user->max_count_point, user->games_played);
    fprintf(file, "%c %c\n", setting_char->snake_char, setting_char->point_char);
    fprintf(file, "%d %d\n", color->snake_fg, color->snake_bg);
    fprintf(file, "%d %d\n", color->point_fg, color->point_bg);
    fprintf(file, "%d %d\n", color->text_fg, color->text_bg);
    fprintf(file, "%d %d\n", color->special_text_fg, color->special_text_bg);
    fclose(file);
    return ;
}


bool read_setting_file(const char* filename, struct setting_char* setting_char, struct user* user, struct color* color){
    FILE* file = fopen(filename, "r");
    int read = 0;
    read += fscanf(file, "%d %d", &user->max_count_point, &user->games_played);
    read += fscanf(file, " %c %c", &setting_char->snake_char, &setting_char->point_char);
    read += fscanf(file, "%d %d", (int*)&color->snake_fg, (int*)&color->snake_bg);
    read += fscanf(file, "%d %d", (int*)&color->point_fg, (int*)&color->point_bg);
    read += fscanf(file, "%d %d", (int*)&color->text_fg, (int*)&color->text_bg);
    read += fscanf(file, "%d %d", (int*)&color->special_text_fg, (int*)&color->special_text_bg);
    fclose(file);
    if(read != 12) return false;
    return true;
}


bool setting_file(const char* filename, struct setting_char* setting_char, struct user* user, struct color* color){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        default_config(setting_char, user, color);
        save_setting_file(filename, setting_char, user, color);
        return true;
    }
    fclose(file);
    bool flg = read_setting_file(filename, setting_char, user, color);
    if(flg) return false;
    return true;
}

void set_color(struct color* color){
    if(!has_colors()){
        init_pair(snake_color, COLOR_WHITE, COLOR_BLACK);
        init_pair(point_color, COLOR_WHITE, COLOR_BLACK);
        init_pair(text_color, COLOR_WHITE, COLOR_BLACK);
        init_pair(special_text_color, COLOR_WHITE, COLOR_BLACK);
        return;
    }
    start_color();
    // 1 number pair, 2 color text, 3 color background
    init_pair(snake_color, all_colors[color->snake_fg], all_colors[color->snake_bg]);
    init_pair(point_color, all_colors[color->point_fg], all_colors[color->point_bg]); 
    init_pair(text_color, all_colors[color->text_fg], all_colors[color->text_bg]); 
    init_pair(special_text_color, all_colors[color->special_text_fg], all_colors[color->special_text_bg]);
}

// --------GRAPHIC------------


void show_list_options(int height, int wigth, int choce_item, struct user* user){
    clear();
    attrset(COLOR_PAIR(text_color));
    for(int i = 0; i < number_item; i++){
        move(((height-number_item)/2 + i), ((wigth - strlen(items[i]))/2));
        if(choce_item == i) attron(A_REVERSE);
        printw("%s", items[i]);
        if(choce_item == i) attroff(A_REVERSE);
    }
    mvprintw(0, 0, "Games played: %d", user->games_played);
    mvprintw(1, 0, "Maximum record: %d", user->max_count_point);
}


void game_over(enum status_snake status_snake, struct Window* window){
    if(status_snake == snake_win){
        int x = (window->low - strlen(win_game))/2;
        int y = (window->row - 1)/2;
        attrset(COLOR_PAIR(special_text_color));
        mvprintw(y, x, "%s", win_game);
    } else if(status_snake == snake_lose){
        int x = (window->low - strlen(lose_game))/2;
        int y = (window->row - 1)/2;
        attrset(COLOR_PAIR(special_text_color));
        mvprintw(y, x, "%s", lose_game);
    }
    mvprintw(window->row/2+1, window->low/2, "total: %d", window->count_point);
    refresh();
    return ;
}


void write_to_display(struct point* point, struct Window* window, struct List* snake, struct setting_char* setting_char){
    clear(); 
    attrset(COLOR_PAIR(text_color));
    mvprintw(0, 0, "%d", window->count_point);
    attrset(COLOR_PAIR(snake_color));
    while(snake){
        move(snake->data.y, snake->data.x);
        addch(setting_char->snake_char);
        snake = snake->next;
    }
    attrset(COLOR_PAIR(point_color));
    move(point->point_y, point->point_x);
    addch(setting_char->point_char);
    refresh();
}


int main_snake(struct setting_char* setting_char){
    struct Window main_window; main_window.count_point = 0;  
    struct point point; 
    timeout(time_wait);           // wait time press
    getmaxyx(stdscr, main_window.row, main_window.low);
    struct List* snake = initialize((type){main_window.low/2, main_window.row/2});
    enum direction direction =  up;
    set_point(&point, &main_window, snake);
    /*
    switch(direction){
        case up:    snake->data.y--; break;
        case down:  snake->data.y++; break;
        case left:  snake->data.x--; break;
        case right: snake->data.x++; break;
    }
    */
    int key;
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
        enum status_snake status_snake = check_snake(&main_window, snake);
        if(status_snake == snake_win || status_snake == snake_lose){
            game_over(status_snake, &main_window);
            sleep(5);
            return main_window.count_point;
        }
        write_to_display(&point, &main_window, snake, setting_char);
    }

    return main_window.count_point;
}


int main(int argv, char** argc){
    struct setting_char setting_char;
    struct user user;
    struct color color;

    bool flg = setting_file(file_name, &setting_char, &user, &color);

    if(flg){
        printf("%s '%s'\n", error_file, file_name);
        return -1;
    }

    srand(time(NULL));
    initscr();                      // init
    cbreak();                       // disables input buffering
    keypad(stdscr, 1);              // setting keyboard
    noecho();                       // disables show input
    curs_set(0);                    // delete cursor

    int height, width;
    getmaxyx(stdscr, height, width);
    
    int key;
    int choice_item = 0;            // choce options number

    set_color(&color);
    show_list_options(height, width, choice_item, &user);

    while((key = getch()) != key_escape){
        switch(key){
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
                    int result = main_snake(&setting_char);
                    if(result >= 1) user.games_played++;
                    if(result > user.max_count_point) user.max_count_point = result;
                    timeout(-1);
                    save_setting_file(file_name, &setting_char, &user, &color);
                } else if (choice_item == 1){
                    break;
                } else if(choice_item == 2){
                    endwin();
                    return 0;
                }
                break;

            case ERR:
                break;
                
            case KEY_RESIZE:
                getmaxyx(stdscr, height, width);
                break;
        }
        show_list_options(height, width, choice_item, &user);
        refresh();
    }

    endwin();                       // return control
    return 0;
}