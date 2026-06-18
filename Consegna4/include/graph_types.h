#ifndef GRAPH_TYPES_H
#define GRAPH_TYPES_H

//////////////////////////////////////////////////
/// Definizione della struttura dati lista
//////////////////////////////////////////////////

/// struct per il nodo della lista
typedef struct node {
    int val; /// prossimo nodo
    float w; /// peso dell'arco
    struct node *next;
} node_t;

/// struct per la lista
typedef struct list {
    node *head;
} list_t;

//////////////////////////////////////////////////
/// Fine Definizione della struttura dati lista
//////////////////////////////////////////////////

#endif
