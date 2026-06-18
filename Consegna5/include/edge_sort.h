#ifndef EDGE_SORT_H
#define EDGE_SORT_H

#include "data_types.h"

// Funzione di supporto per l'ordinamento mediante quicksort
void edge_swap(arco& a, arco& b);

// Subroutine partition di quicksort
int edge_partition(arco* edges, int left, int right);

// Utilizzo quicksort per ordinare l'array di archi
// Rappresenta il primo step di Kruskal
void edge_quicksort(arco* edges, int left, int right);

#endif
