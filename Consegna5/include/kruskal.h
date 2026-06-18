#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <string>

#include "data_types.h"

void print_cluster(int* parent, std::string* city, int N, int root);
void kruskal_clustering(arco* edges, int n_edges, std::string* city, int N);

#endif
