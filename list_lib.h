#ifndef LIST_LIB_H
#define LIST_LIB_H
enum direction{up, down, left, right};
typedef struct coord_snake{
    int x, y;
} type;

struct List{
    type data;
    struct List *next;
};

struct List* initialize(type n);
struct List* iteration(struct List* first, int i);
void delete_List(struct List* first);
struct List* get_last(struct List* first);
void push_back(struct List* first, type n);
void pop_back(struct List** first);
void push_begin(struct List** first, type n);
void pop_begin(struct List** first);
int len_list(struct List* first);

#endif