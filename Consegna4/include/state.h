#ifndef STATE_H
#define STATE_H

#include <fstream>

#include "graph_types.h"

using namespace std;

#define INFTY 1000000

extern int ct_swap;
extern int ct_cmp;
extern int ct_op; /// operazioni per la ricerca

extern int max_dim;
extern int ntests;
extern int ndiv;
extern int details;
extern int graph;

/// file di output per grafo
extern ofstream output_graph;
extern int n_operazione; /// contatore di operazioni per visualizzare i vari step

extern int ct_visit; // contatore durante visita

//////////////////////////////////////////////////
/// Definizione della struttura dati grafo
//////////////////////////////////////////////////

extern int *V;          // elenco dei nodi del grafo
extern int *V_visitato; // nodo visitato?
extern int *V_prev;     // nodo precedente dalla visita
extern float *V_dist;   // distanza da sorgente

// list_t* E;  /// array con le liste di adiacenza per ogni nodo
extern list_t **E; /// array di puntatori a le liste di adiacenza per ogni nodo
extern int n_nodi;

//////////////////////////////////////////////////
/// Fine Definizione della struttura dati grafo
//////////////////////////////////////////////////

extern list_t *global_ptr_ref; /// usato per memorizzare il puntatore alla prima lista allocata

extern int *heap;     /// heap[i] = codice del nodo del grafo in posizione i
extern int *heap_pos; /// heap_pos[nodo] = posizione corrente del nodo dentro heap[]. Serve per trovare in O(1) la posizione di un nodo dentro il min heap
extern int heap_size; /// dimensione attuale dell'heap

#endif
