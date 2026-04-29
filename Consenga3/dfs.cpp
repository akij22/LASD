#include "dfs.h"

#include <cstdio>
#include <iostream>


// graph = grafo
// n = indice del nodo corrente
// details = flag per stampare informazioni dettagliate durante l'esecuzione
// dot = contesto per generare il grafo su Dot passo-passo

using namespace std;

int dfs(Graph *graph, int n, bool details, DotContext *dot) {
    if (details) {
        std::printf(
            "DFS: lavoro sul nodo %d (visitato %d)\n",
            n,
            graph->V_visitato[n]
        );
    }

    // Se il nodo corrente è già in visita, allora ho trovato un ciclo, quindi lo segnalo
    if (graph->V_visitato[n] == 1) {
        return 1;
    }

    // Se il nodo corrente è già stato completamente esplorato (NERO), termino (ho individuato un DAG)
    if (graph->V_visitato[n] == 2) {
        return 0;
    }

    // Se il nodo corrente è bianco, allora lo coloro di grigio
    graph->V_visitato[n] = 1;

    if (details) {
        std::printf("Visito il nodo %d (val %d)\n", n, graph->V[n]);
    }

    // Contatore per individuare tutti i cicli nel grafo
    int ret = 0;

    // Esploro la lista di adiacenza del nodo corrente e prendo il primo nodo presente nella sua lista di adiacenza
    node_t *elem = graph->E[n]->head;

    while (elem != nullptr) {

        // Disegno l'intero grafo nell'istante corrente
        dot_print_graph(dot, graph);

        // Disegno l'arco uscente da `n` verso il nodo corrente `elem->val`
        dot_print_dfs_edge(
            dot,
            n,
            elem->val,
            graph->V_visitato[elem->val] != 0
        );

        ret += dfs(graph, elem->val, details, dot);
        elem = elem->next;
    }

    // Una volta completata tutta la lista di adiacenza di n, lo coloro di nero
    graph->V_visitato[n] = 2;
    dot_print_graph(dot, graph);

    return ret;
}

// `seq` = array contenente la sequenza di valori che si sta ricercando all'interno del grafo come cammino
// `index` = indice per scorrere l'array `seq` e confrontare ogni volta il valore dell'array con il valore del nodo attuale
void DFS_consegna3(Graph *graph, int n, int *seq, int n_elem_seq, int index, bool &found, bool details, DotContext *dot) {

    if (found) {
        return;
    }

    if (details) {
        printf(
            "DFS: lavoro sul nodo %d (visitato %d)\n",
            n,
            graph->V_visitato[n]
        );
    }

    graph->V_visitato[n] = 1;

    if (details) {
        printf("Visito il nodo %d (val %d)\n", n, graph->V[n]);
    }

    // Se il valore del nodo corrente è uguale al corrispettivo valore nell'array, aggiorno i contatori
    cout << "Valore attuale da ricercare: " << seq[index] << std::endl;
    cout << "Valore attuale di `index`: " << index << std::endl;

    if (graph->V[n] != seq[index]) {
        cout << "Nodo " << graph->V[n] << " scartato..." << std::endl;
        index = 0;
        (void)index;
        return;
    } else {
        cout << "Nodo corretto trovato!" << std::endl;
        index++;
    }

    if (index == n_elem_seq) {
        found = true;
        return;
    }

    node_t *elem = graph->E[n]->head;

    if (elem == nullptr) {
        graph->V_visitato[n] = 2;
        return;
    }

    while (elem != nullptr) {
        dot_print_graph(dot, graph);

        dot_print_dfs_edge(dot, n, elem->val, graph->V_visitato[elem->val] != 0);

        // Aggiorno il contatore "globale" e chiamo DFS
        cout << "Nodo da esplorare: " << elem->val << std::endl;
        DFS_consegna3(graph, elem->val, seq, n_elem_seq, index, found, details, dot);

        elem = elem->next;
    }

    graph->V_visitato[n] = 2;
    dot_print_graph(dot, graph);
}
