#ifndef LIST_H
#define LIST_H

#include "graph_types.h"

int get_address(void *node);
void list_print(list_t *l);
list_t *list_new(void);
void list_insert_front(list_t *l, int elem, float w);

#endif
