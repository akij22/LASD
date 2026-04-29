#pragma once

#include "dot_output.h"
#include "graph.h"

int dfs(Graph *graph, int node_index, bool details, DotContext *dot);
void DFS_consegna3(
    Graph *graph,
    int n,
    int *seq,
    int n_elem_seq,
    int index,
    bool &found,
    bool details,
    DotContext *dot
);
