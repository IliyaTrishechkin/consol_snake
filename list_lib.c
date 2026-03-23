#include <stdio.h>
#include <stdlib.h>
#include "list_lib.h"


struct List* initialize(type n){
    struct List* tmp = malloc(sizeof(struct List));
    tmp->data = n;
    tmp->next = NULL;
    return tmp;
}


void delete_List(struct List* first){
    while(first){
        struct List* tmp = first->next;
        free(first);
        first = tmp;
    }
    return ;
}

struct List* get_last(struct List* first){
    if(!first){return NULL;}
    while(first->next){
        first = first->next;
    }
    return first;
}


int len_list(struct List* first){
    if(!first){return 0;}
    int len = 1;
    while(first->next != NULL){
        len++;
        first = first->next;
    }
    return len;
}


struct List* iteration(struct List* first, int i){
    if(!first){return NULL;}
    if(i<0){return NULL;}
    if(i > len_list(first)){return NULL;}
    while(i != 0){
        first = first->next;
        i--;
    }
    return first;
}


void push_back(struct List* first, type n){
    if(!first){return ;}
    struct List *last = get_last(first), *tmp;
    tmp = malloc(sizeof(struct List));
    tmp->data = n;
    tmp->next = NULL;
    last->next = tmp;
}


void pop_back(struct List** first){
    if(!(*first)){return ;}
    if(!(*first)->next){
        delete_List(*first);
        *first = NULL;
        return ;
    }
    struct List* tmp = *first;
    while(tmp->next->next){
        tmp = tmp->next;
    }
    free(tmp->next);
    tmp->next = NULL;
    return ;
}


void push_begin(struct List** first, type n){
    if(!(*first)){return ;}
    struct List *tmp = malloc(sizeof(struct List));
    tmp->data = n;
    tmp->next = *first;
    *first = tmp;
}


void pop_begin(struct List** first){
    if(!(*first)){return ;}
    if(!(*first)->next){
        delete_List(*first);
        *first = NULL;
        return ;
    }
    struct List *tmp = *first, *tmp2 = (*first)->next;
    free(tmp);
    *first = tmp2;

}
