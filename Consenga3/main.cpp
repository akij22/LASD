#include "dfs.h"
#include "dot_output.h"
#include "graph.h"
#include "options.h"

#include <iostream>

int main(int argc, char **argv) {
    ProgramOptions options = options_default();

    if (!parse_cmd(argc, argv, &options)) {
        return 1;
    }

    DotContext dot;
    dot_init(&dot);

    if (options.graph && !dot_open(&dot, "graph.dot")) {
        std::cerr << "Errore: impossibile aprire graph.dot in scrittura\n";
        return 1;
    }

    Graph graph;
    graph.n_nodi = 6;
    graph.V = new int[graph.n_nodi];
    graph.V_visitato = new int[graph.n_nodi];
    graph.E = new list_t *[graph.n_nodi];

    for (int i = 0; i < graph.n_nodi; i++) {
        graph.V[i] = i;
        graph.V_visitato[i] = 0;
        graph.E[i] = list_new(options.verbose);
    }

    list_insert_front(graph.E[0], 2);
    list_insert_front(graph.E[0], 1);
    list_insert_front(graph.E[2], 5);
    list_insert_front(graph.E[2], 4);
    list_insert_front(graph.E[2], 3);

    const void *address_ref = graph.E[0];

    dot_print_graph(&dot, &graph);
    graph_print_adjacency(&graph, options.verbose, address_ref);

    int seq[3] = {0, 2, 5};
    int n_elem_seq = 3;
    bool found = false;

    DFS_consegna3(&graph, 0, seq, n_elem_seq, 0, found, options.verbose, &dot);

    std::cout
        << (found ? "Trovata sequenza nel grafo" : "Non e' stata trovata sequenza nel grafo")
        << std::endl;

    if (options.graph) {
        dot_close(&dot);
        std::cout << " File graph.dot scritto" << std::endl
                  << "Creare il grafo con: dot graph.dot -Tpdf -o graph.pdf" << std::endl;
    }

    graph_destroy(&graph);

    return 0;
}
