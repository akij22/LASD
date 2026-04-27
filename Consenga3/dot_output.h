// File per la generazione del grafo in formato dot

#pragma once

#include "graph.h"

#include <fstream>

struct DotContext {
    bool enabled;
    std::ofstream output;
    int operation;
    int visit_counter;
};

void dot_init(DotContext *dot);
bool dot_open(DotContext *dot, const char *filename);
void dot_close(DotContext *dot);
void dot_print_graph(DotContext *dot, const Graph *graph);
void dot_print_dfs_edge(DotContext *dot, int from, int to, bool already_visited);
