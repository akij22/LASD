#include "dot_output.h"

static void dot_print_node(DotContext *dot, const Graph *graph, int index) {
    dot->output << "node_" << index << "_" << dot->operation << '\n';
    dot->output << "[ shape = oval; ";

    if (graph->V_visitato[index] == 1) {
        dot->output << "fillcolor = \"#ff0000\"; style=filled; ";
    }

    if (graph->V_visitato[index] == 2) {
        dot->output << "fillcolor = \"#555555\"; style=filled; ";
    }

    dot->output << "label = "
                << "\"Idx: " << index << ", val: " << graph->V[index] << "\" ];\n";

    // Prendo la lista di adiacenza del nodo corrente (`index`)
    node_t *elem = graph->E[index]->head;

    while (elem != nullptr) {

        // Creo l'arco uscente da `index`
        dot->output << "node_" << index << "_" << dot->operation << " -> ";
        dot->output << "node_" << elem->val << "_" << dot->operation << " [  color=gray ]\n";
        elem = elem->next;
    }
}

void dot_init(DotContext *dot) {
    dot->enabled = false;
    dot->operation = 0;
    dot->visit_counter = 0;
}

bool dot_open(DotContext *dot, const char *filename) {
    dot->output.open(filename);

    if (!dot->output.is_open()) {
        return false;
    }

    dot->enabled = true;
    dot->output << "digraph g" << '\n';
    dot->output << "{ " << '\n';
    dot->output << "node [shape=none]" << '\n';
    dot->output << "rankdir=\"LR\"" << '\n';
    return true;
}

void dot_close(DotContext *dot) {
    if (!dot->enabled) {
        return;
    }

    dot->output << "}" << '\n';
    dot->output.close();
    dot->enabled = false;
}

void dot_print_graph(DotContext *dot, const Graph *graph) {
    if (!dot->enabled) {
        return;
    }

    for (int i = 0; i < graph->n_nodi; i++) {
        dot_print_node(dot, graph, i);
    }

    dot->operation++;
}

// Stampa di un singolo arco tra due nodi (`from` --> `to`)
void dot_print_dfs_edge(DotContext *dot, int from, int to, bool already_visited) {
    if (!dot->enabled) {
        return;
    }

    dot->output << "dfs_" << from << " -> dfs_" << to;

    if (already_visited) {
        dot->output << "[color=gray, label = \"" << dot->visit_counter++ << "\"]";
    } else {
        dot->output << "[color=red, label = \"" << dot->visit_counter++ << "\"]";
    }

    dot->output << '\n';
}
