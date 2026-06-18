#ifndef MIN_HEAP_H
#define MIN_HEAP_H

int parent_idx(int n);
int child_L_idx(int n);
int child_R_idx(int n);
void heap_swap(int i, int j);
int heap_node_has_lower_dist(int i, int j);
void heapify_up(int i);
void heap_insert(int nodo);
int is_leaf(int n);
void heapify_down(int i);
int heap_remove_min();
void decrease_key(int nodo);
void heap_init();

#endif
