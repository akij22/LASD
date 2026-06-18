#ifndef LEGACY_TYPES_H
#define LEGACY_TYPES_H

typedef struct list my_stack;

//////////////////////////////////////////////////
/// Definizione della struttura dati lista
//////////////////////////////////////////////////

/// struct per il nodo della lista
typedef struct node {
    int val;
    struct node *next;
} node_t;

/// struct per la lista
typedef struct list {
    node *head;
    // node* tail; /// per lista doubly linked
} list_t;

//////////////////////////////////////////////////
/// Fine Definizione della struttura dati lista
//////////////////////////////////////////////////

#endif
