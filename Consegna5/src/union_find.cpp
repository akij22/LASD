#include "union_find.h"

// Dato un nodo x appartenente ad un insieme, restituisce il rappresentante del suo insieme
int find_set(int* parent, int x) {

    // Se il nodo attuale non è il rappresentante, ricorsivamente trova il rappresentante del suo insieme
    if (parent[x] != x)
        parent[x] = find_set(parent, parent[x]);
    return parent[x];
}

void union_set(int* parent, int* rank, int a, int b) {
    int root_a = find_set(parent, a);
    int root_b = find_set(parent, b);


    if (root_a == root_b)
        return;

    // Se root_a è un albero con altezza inferiore a quella di root_b, attacco la "partizione a" sotto quella di b
    if (rank[root_a] < rank[root_b]) {
        parent[root_a] = root_b;

    } else if (rank[root_a] > rank[root_b]) {
        parent[root_b] = root_a;
    } else {

        // Se gli albero hanno la stessa altezza, attacco root_b sotto root_a e incremento il rank di root_a
        parent[root_b] = root_a;
        rank[root_a]++;
    }
}
