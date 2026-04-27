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
    graph_init_cycle(&graph, 7, options.verbose);

    const void *address_ref = graph.E[0];

    dot_print_graph(&dot, &graph);
    graph_print_adjacency(&graph, options.verbose, address_ref);

    std::cout << "ciclo? " << dfs(&graph, 0, options.verbose, &dot) << "\n";

    if (options.graph) {
        dot_close(&dot);
        std::cout << " File graph.dot scritto" << std::endl
                  << "Creare il grafo con: dot graph.dot -Tpdf -o graph.pdf" << std::endl;
    }

    graph_destroy(&graph);

    return 0;
}
