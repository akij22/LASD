// File per la definizione e generazione del grafo

#pragma once

#include "linked_list.h"


// Struttura dati grafo, riorganizzata partdendo dal file di partenza
struct Graph {
    int n_nodi;

    // Elenco dei nodi del grafo
    int *V;

    // Array di interi per memorizzare i nodi non visitati / in visita / completamente visitati
    // 0 = non visitato (BIANCO), 1 = in visita (GRIGIO), 2 = completamente visitato (NERO)
    int *V_visitato;

    // Lista di adiacenza (array in cui ogni cella contiene una lista)
    // cella = singolo nodo, lista associata ad una cella = nodi adiacenti al "nodo cella" corrente
    list_t **E;
};

void graph_init_cycle(Graph *graph, int node_count, bool details);
void graph_destroy(Graph *graph);
void graph_print_adjacency(const Graph *graph, bool details, const void *address_ref);
