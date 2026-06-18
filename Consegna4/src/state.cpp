#include "../include/state.h"

int ct_swap = 0;
int ct_cmp = 0;
int ct_op = 0; /// operazioni per la ricerca

int max_dim = 0;
int ntests = 1;
int ndiv = 1;
int details = 0;
int graph = 0;

/// file di output per grafo
ofstream output_graph;
int n_operazione = 0; /// contatore di operazioni per visualizzare i vari step

int ct_visit = 0; // contatore durante visita

int *V;          // elenco dei nodi del grafo
int *V_visitato; // nodo visitato?
int *V_prev;     // nodo precedente dalla visita
float *V_dist;   // distanza da sorgente

// list_t* E;  /// array con le liste di adiacenza per ogni nodo
list_t **E; /// array di puntatori a le liste di adiacenza per ogni nodo
int n_nodi;

list_t *global_ptr_ref = NULL; /// usato per memorizzare il puntatore alla prima lista allocata

int *heap;     /// heap[i] = codice del nodo del grafo in posizione i
int *heap_pos; /// heap_pos[nodo] = posizione corrente del nodo dentro heap[]. Serve per trovare in O(1) la posizione di un nodo dentro il min heap
int heap_size = 0; /// dimensione attuale dell'heap
