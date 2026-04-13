#ifndef QUEUE_H
#define QUEUE_H

#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct node node_t;

typedef struct element {
    node_t* val;
    struct element* next;
} element_t;

typedef struct queue {
    element_t* head;
    element_t* tail;
} queue_t;


inline void queue_init(queue_t* q) {
    q->head = NULL;
    q->tail = NULL;
}

inline bool queue_is_empty(queue_t* q) {
    return q->head == NULL;
}

inline void enqueue(queue_t* q, node_t* value) {
    element_t* new_element = new element_t;
    new_element->val = value;
    new_element->next = NULL;

    if (queue_is_empty(q)) {
        q->head = new_element;
        q->tail = new_element;
        return;
    }

    q->tail->next = new_element;
    q->tail = new_element;
}

inline node_t* dequeue(queue_t* q) {
    if (queue_is_empty(q))
        return NULL;

    element_t* first = q->head;
    node_t* value = first->val;

    q->head = first->next;
    if (q->head == NULL)
        q->tail = NULL;

    delete first;
    return value;
}

void print_queue(queue_t* q);


#endif
