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

node_t *node_new(int elem);
void node_print(node_t *n);

void tree_insert_child_L(node_t *n, int elem);
void tree_insert_child_R(node_t *n, int elem);

void preOrder(node_t *n);
void postOrder(node_t *n);

int calculate_node_height(node_t *current);

int checkHeight(node_t *current);

bool isBalanced(node_t *root);

inline void print_tree_rec(node_t *n, int depth) {
    if (n == NULL)
        return;

    print_tree_rec(n->R, depth + 1);

    for (int i = 0; i < depth; i++)
        printf("    ");
    printf("%d\n", n->val);

    print_tree_rec(n->L, depth + 1);
}

inline void print_tree(node_t *root) {
    if (root == NULL) {
        printf("Albero vuoto\n");
        return;
    }

    print_tree_rec(root, 0);
    printf("\n");
}

inline node_t *flip(node_t *n) {
    node_t *n1 = node_new(n->val);

    if (n->L != NULL)
        n1->R = flip(n->L);

    if (n->R != NULL)
        n1->L = flip(n->R);

    return n1;
}

inline int calculate_node_dept(node_t *n, node_t *current) {
    if (current == NULL)
        printf("Ho trovato un nodo null, torno indietro\n");

    if (current == NULL)
        return -1;

    if (current == n)
        return 0;

    int left_counter = calculate_node_dept(n, current->L);

    if (left_counter != -1)
        return 1 + left_counter;

    int right_counter = calculate_node_dept(n, current->R);

    if (right_counter != -1)
        return right_counter + 1;

    return -1;
}

inline void calculate_all_depts(node_t *current, node_t *root) {
    if (current == NULL)
        return;

    printf("Profondita' del nodo %i e' %i \n", current->val, calculate_node_dept(current, root));

    calculate_all_depts(current->L, root);
    calculate_all_depts(current->R, root);
}



inline void calculate_all_heights(node_t *current) {
    if (current == NULL)
        return;

    printf("Altezza' del nodo %i e' %i \n", current->val, calculate_node_height(current));

    calculate_all_heights(current->L);
    calculate_all_heights(current->R);
}



inline node_t *LCA(node_t *current, node_t *u, node_t *v) {
    if (current == NULL)
        return NULL;

    if (current->val == u->val || current->val == v->val)
        return current;

    node_t *left = LCA(current->L, u, v);
    node_t *right = LCA(current->R, u, v);


    // Se trovo entrambi i nodi ricercati nei due sottoalberi (sx e dx), allora ritorno il nodo corrente
    if (left != NULL && right != NULL)
        return current;

    if (left != NULL)
        return left;
    if (right != NULL)
        return right;

    return NULL;
}

#endif
