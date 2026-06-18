#include "edge_sort.h"

// Funzione di supporto per l'ordinamento mediante quicksort
void edge_swap(arco& a, arco& b) {
    arco tmp = a;
    a = b;
    b = tmp;
}


// Subroutine partition di quicksort
int edge_partition(arco* edges, int left, int right) {
    float pivot = edges[right].w;
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (edges[j].w <= pivot) {
            i++;
            edge_swap(edges[i], edges[j]);
        }
    }

    edge_swap(edges[i + 1], edges[right]);
    return i + 1;
}

// Utilizzo quicksort per ordinare l'array di archi
// Rappresenta il primo step di Kruskal
void edge_quicksort(arco* edges, int left, int right) {
    if (left >= right)
        return;

    int pivot_idx = edge_partition(edges, left, right);
    edge_quicksort(edges, left, pivot_idx - 1);
    edge_quicksort(edges, pivot_idx + 1, right);
}
