#ifndef UNION_FIND_H
#define UNION_FIND_H

// Dato un nodo x appartenente ad un insieme, restituisce il rappresentante del suo insieme
int find_set(int* parent, int x);

void union_set(int* parent, int* rank, int a, int b);

#endif
