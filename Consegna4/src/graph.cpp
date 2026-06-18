#include "../include/graph.h"
#include "../include/list.h"
#include "../include/state.h"

void init_empty_graph(int nodi) {
    n_nodi = nodi;

    V = new int[n_nodi];
    V_visitato = new int[n_nodi];
    V_prev = new int[n_nodi];
    V_dist = new float[n_nodi];
    heap = new int[n_nodi];
    heap_pos = new int[n_nodi];
    E = new list_t *[n_nodi];

    for (int i = 0; i < n_nodi; i++) {
        V[i] = i;
        V_visitato[i] = 0;
        V_prev[i] = -1;
        V_dist[i] = INFTY;
        E[i] = list_new();

        if (i == 0)
            global_ptr_ref = E[i];
    }
}

void reset_shortest_path_data() {
    for (int i = 0; i < n_nodi; i++) {
        V_visitato[i] = 0;
        V_prev[i] = -1;
        V_dist[i] = INFTY;
    }
    heap_size = 0;
}

int DFS(int n) {

    if (details)
        printf("DFS: lavoro sul nodo %d (visitato %d)\n", n, V_visitato[n]);

    if (V_visitato[n])
        return 0;

    V_visitato[n] = 1; // prima volta che incontro questo nodo

    if (details)
        printf("Visito il nodo %d (val %d)\n", n, V[n]);

    /// esploro la lista di adiacenza
    node_t *elem = E[n]->head;
    while (elem != NULL) { /// elenco tutti i nodi nella lista

        /// espando arco  n --> elem->val
        /// quindi DFS(elem->val)
        output_graph << "dfs_" << n << " -> dfs_" << elem->val;
        if (V_visitato[elem->val])
            output_graph << "[color=gray, label = \"" << ct_visit++ << "\"]";
        else
            output_graph << "[color=red, label = \"" << ct_visit++ << "\"]";
        output_graph << endl;

        if (V_visitato[elem->val] == 1)
            return 1;

        int ret = DFS(elem->val);

        if (ret == 1)
            return 1;

        elem = elem->next;
    }

    V_visitato[n] = 2; // abbandono il nodo per sempre
    return 0;
}
