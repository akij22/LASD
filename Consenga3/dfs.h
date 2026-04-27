#pragma once

#include "dot_output.h"
#include "graph.h"

int dfs(Graph *graph, int node_index, bool details, DotContext *dot);
