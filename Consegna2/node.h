#ifndef NODE_H
#define NODE_H

#include <fstream>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node *L;
    struct node *R;
} node_t;

#endif
