#include "graph.h"

#include <cstdio>

void graph_init_cycle(Graph *graph, int node_count, bool details) {
    graph->n_nodi = node_count;
    graph->V = new int[node_count];
    graph->V_visitato = new int[node_count];
    graph->E = new list_t *[node_count];

    for (int i = 0; i < node_count; i++) {
        graph->V[i] = 2 * i;
        graph->V_visitato[i] = 0;
        graph->E[i] = list_new(details);
    }

    for (int i = 0; i < node_count; i++) {
        list_insert_front(graph->E[i], (i + 1) % node_count);
    }
}

void graph_destroy(Graph *graph) {
    for (int i = 0; i < graph->n_nodi; i++) {
        list_delete(graph->E[i]);
    }

    delete[] graph->E;
    delete[] graph->V_visitato;
    delete[] graph->V;

    graph->n_nodi = 0;
    graph->V = nullptr;
    graph->V_visitato = nullptr;
    graph->E = nullptr;
}

void graph_print_adjacency(const Graph *graph, bool details, const void *address_ref) {
    for (int i = 0; i < graph->n_nodi; i++) {
        std::printf("Sono il nodo di indice %d nell'array\n", i);
        std::printf("Il valore del nodo e' %d\n", graph->V[i]);
        std::printf("La lista di adiacenza e': ");
        list_print(graph->E[i], details, address_ref);
    }
}
