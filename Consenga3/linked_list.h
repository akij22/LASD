#pragma once

struct node_t {
    int val;
    node_t *next;
};

struct list_t {
    node_t *head;
};

list_t *list_new(bool details);
void list_delete(list_t *list);
void list_insert_front(list_t *list, int elem);
void list_print(const list_t *list, bool details, const void *address_ref);
