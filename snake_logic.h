#ifndef SNAKE_LOGIC_H
#define SNAKE_LOGIC_H

enum status_snake{snake_lose, snake_win, snake_none};

// save all information about window object
struct Window{
    int row; // Oy or Y
    int low; // Ox or X
    int count_point; // count point
};

struct point{
    int point_x, point_y; // coord apple
};

void set_point(struct point* point, struct Window* window, struct List* snake);
void move_snake(enum direction direction, struct Window* window, struct List* snake);
void point_check(struct point* point, struct Window* window, struct List* snake, enum direction direction);
enum status_snake check_snake(struct Window* window, struct List* snake);

#endif