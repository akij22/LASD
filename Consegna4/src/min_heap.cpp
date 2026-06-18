#include <stdio.h>

#include "../include/min_heap.h"
#include "../include/state.h"

//////////////////////////////////////////////////
/// Inizio Definizione del MIN HEAP
//////////////////////////////////////////////////

// RECUPERO DEL PADRE E DEI FIGLI MEDIANTE OPERAZIONI ARITMETICHE
// Esse sono assicurate mediante l'implementazione scelta del min-heap, utilizzando un array con indici

/// uso -1 per indicare un indice non esistente
int parent_idx(int n) {
    if (n == 0)
        return -1;
    return (n - 1) / 2;
}

int child_L_idx(int n) {
    if (2 * n + 1 >= heap_size)
        return -1;
    return 2 * n + 1;
}

int child_R_idx(int n) {
    if (2 * n + 2 >= heap_size)
        return -1;
    return 2 * n + 2;
}

void heap_swap(int i, int j) {
    int t = heap[i];
    heap[i] = heap[j];
    heap[j] = t;

    heap_pos[heap[i]] = i;
    heap_pos[heap[j]] = j;
}

int heap_node_has_lower_dist(int i, int j) {
    return V_dist[heap[i]] < V_dist[heap[j]];
}

// heapify_up ora non confronta più i valori interi dei nodi, bensì confronta le distanze effettive
void heapify_up(int i) {

    while (i != 0) {
        int parent = parent_idx(i);

        // Se il padre ha una distanza minore, non devo eseguire alcun swap per la proprietà del min-heap
        if (!heap_node_has_lower_dist(i, parent)) {
            if (details)
                printf("Il genitore %d ha distanza %.2f <= del nodo %d con distanza %.2f, esco\n",
                       heap[parent], V_dist[heap[parent]], heap[i], V_dist[heap[i]]);
            return;
        }

        if (details)
            printf("Il genitore %d ha distanza %.2f > del nodo %d con distanza %.2f, swap\n",
                   heap[parent], V_dist[heap[parent]], heap[i], V_dist[heap[i]]);

        // Aggiusto gli indici dei nodi all'interno della "mappa" `heap_pos`
        heap_swap(parent, i);
        i = parent;
    }
}


void heap_insert(int nodo) {

    if (details)
        printf("Inserisco nodo %d in posizione %d\n", nodo, heap_size);

    if (heap_size < n_nodi) {

        int i = heap_size;
        heap_size++;

        heap[i] = nodo;
        heap_pos[nodo] = i;

        // Dopo l'inserimento, devo assicurarmi che la proprietà del min-heap sia rispettata
        heapify_up(i);

    } else {
        printf("Heap pieno!\n");
    }
}

/// restituisce 0 se il nodo in posizione n e' un nodo interno (almeno un figlio)
/// restituisce 1 se il nodo non ha figli
int is_leaf(int n) {
    return (child_L_idx(n) == -1);

    /* versione equivalente
    if (child_L_idx(n)==-1)
      return 1;
    return 0;
    */
}


void heapify_down(int i) {
    while (!is_leaf(i)) {
        if (details)
            printf("Lavoro con il nodo %d in posizione heap %d, distanza %.2f\n",
                   heap[i], i, V_dist[heap[i]]);

        int figlio_left = child_L_idx(i);
        int figlio_right = child_R_idx(i);
        int figlio_min = figlio_left;

        if (figlio_right != -1 && heap_node_has_lower_dist(figlio_right, figlio_left))
            figlio_min = figlio_right;

        if (!heap_node_has_lower_dist(figlio_min, i))
            break;

        heap_swap(i, figlio_min);
        i = figlio_min;
    }
}

int heap_remove_min() {

    if (heap_size <= 0) { /// heap vuoto
        printf("Errore: heap vuoto\n");
        return -1;
    }

    int minimo = heap[0];

    if (details)
        printf("Minimo identificato: nodo %d con distanza %.2f\n", minimo, V_dist[minimo]);

    heap_pos[minimo] = -1;
    heap_size--;

    if (heap_size > 0) {

        // Inserisco l'ultimo nodo dell'heap in cima e poi lo sposto verso il basso per ripristinare la proprietà del min-heap
        heap[0] = heap[heap_size];
        heap_pos[heap[0]] = 0;

        // Spostamento verso il basso del nodo inserito come radice
        heapify_down(0);
    }

    return minimo;
}


// decrease_key viene invocato quando si calcola una nuova distanza per un nodo all'interno del grafo
// Quindi, devo confrontare tale nuova distanza `V_dist[nodo]` con le distanze dei nodi presenti nell'heap e, in caso, effettuare uno swap
void decrease_key(int nodo) {

    // Recupero l'indice del nodo all'interno del min heap (mediante la "mappa" che mi sono costruito)
    int indice_heap = heap_pos[nodo];

    if (indice_heap < 0 || indice_heap >= heap_size) {
        printf("Nodo non esistente\n");
        return;
    }

    // Aggiusto il min heap, in caso ce ne sia bisogno
    heapify_up(indice_heap);
}

void heap_init() {
    heap_size = 0;

    for (int i = 0; i < n_nodi; i++)
        heap_pos[i] = -1;

    for (int i = 0; i < n_nodi; i++)
        heap_insert(i);
}


//////////////////////////////////////////////////
/// Fine Definizione del MIN HEAP
//////////////////////////////////////////////////
