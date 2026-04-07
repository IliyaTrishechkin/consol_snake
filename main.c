#include <ncurses.h>
#include <string.h>
#include "snake_logic.h"

enum {time_wait = 100};
enum {key_escape = 27};
enum colors_object {snake = 1, point = 2, text = 3, special_text = 4};
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

// --------GRAPHIC------------


void show_list_options(int height, int wigth, int choce_item){
    for(int i = 0; i < number_item; i++){
        move(((height-number_item)/2 + i), ((wigth - strlen(items[i]))/2));
        if(choce_item == i) attron(A_REVERSE);
        printw("%s", items[i]);
        if(choce_item == i) attroff(A_REVERSE);
    }
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

    //printf("%c %c\n", setting_char.snake_char, setting_char.point_char);

    return 0;

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
                    timeout(-1);
                } else if(choice_item == 2){
                    endwin();
                    return 0;
                }
                break;
        }
        show_list_options(height, width, choice_item);
        refresh();
    }

    endwin();                       // return control
    return 0;
}