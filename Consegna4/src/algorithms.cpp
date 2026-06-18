#include <stdio.h>

#include "../include/algorithms.h"
#include "../include/min_heap.h"
#include "../include/state.h"

void shortest_path(int n) {

    // Il nodo da cui parto ha, per definizione, DISTANZA 0
    // V_dist[i] = costo minimo per arrivare dal nodo sorgente al nodo i
    V_dist[n] = 0;

    // Inizializzazione del min heap
    heap_init();

    while (heap_size != 0) {

        // Estraggo il minimo in O(logn), dato che:
        // * Estraggo il minimo in O(1)
        // * Mantengo la proprità del min-heap, eseguendo heapify down in O(logn) = altezza dell'albero
        int u = heap_remove_min();

        /// I nodi rimanenti sono irraggiungibili dalla sorgente.
        if (u < 0 || V_dist[u] == INFTY)
            break;

        // Lo marco come elaborato
        V_visitato[u] = 1;

        // Esploro la lista di adiacenza del nodo appena individuato
        node_t *elem = E[u]->head;

        while (elem != NULL) {
            int v = elem->val; /// arco u --> v

            /// alt ← dist[u] + Graph.Edges(u, v)
            /// Aggiungo alla distanza da nodo sorgente a u il peso dell'arco u --> v (elem->w)
            float alt = V_dist[u] + elem->w; /// costo per arrivare al nuovo nodo passando per u
            if (V_visitato[v] == 0 && alt < V_dist[v]) {
                V_dist[v] = alt;

                // Ricostruisco il cammino minimo, salvandomi il nodo precedente per poter ricostruire il cammino
                V_prev[v] = u;

                // Invoco `decrease_key`, perche ora il nodo di cui ho modificato la distanza potrebbe
                // dover salire nel min heap (quindi se non eseguo tale "aggiornamento" una futura estrazione
                // del nodo con distanza minima potrebbe risultare errata)
                decrease_key(v);
            }
            elem = elem->next;
        }

    }

}

void bellman_ford(int n) {

    // Il nodo da cui parto ha, per definizione, DISTANZA 0
    // V_dist[i] = costo minimo conosciuto per arrivare dal nodo sorgente al nodo i
    V_dist[n] = 0;

    // Bellman-Ford rilassa tutti gli archi per n_nodi - 1 volte.
    // Dopo al massimo n_nodi - 1 archi, un cammino minimo semplice e' completo.
    //
    // Ciclo esterno = ciclo che determina le passate
    for (int i = 0; i < n_nodi - 1; i++) {
        int modificato = 0;


        // Scorro tutti i nodi
        //
        // Ciclo interno = ciclo che scorre tutti i nodi del grafo
        for (int u = 0; u < n_nodi; u++) {

            // Se ho una distanza che non è infinito, allora posso rilassare gli archi
            if (V_dist[u] != INFTY) {

                node_t *elem = E[u]->head;
                while (elem != NULL) {
                    int v = elem->val; /// arco u --> v

                    // Calcolo costo dell'arco u --> v
                    float alt = V_dist[u] + elem->w;

                    // Rilassamento
                    if (alt < V_dist[v]) {
                        V_dist[v] = alt;
                        V_prev[v] = u;

                        // Mi annoto che ho modificato una distanza, quindi dovrò
                        // rieseguire una ulteriore passata
                        modificato = 1;
                    }

                    elem = elem->next;
                }
            }

            // se l'arco ha peso infinito, allora non posso rilassare nulla
            // (dipende tutto dall'ordine di estrazione dei nodi)

        }

        // Se in una passata non ho cambiato alcuna distanza, allora le distanze minime sono
        // già finali, quindi posso terminare l'algoritmo
        if (!modificato)
            break;
    }

    // Controllo presenza di cicli negativi
    // Se dopo |V|-1 iterazioni trovo ancora un possibile rilassamento da eseguire, allora c'è un ciclo negativo
    for (int u = 0; u < n_nodi; u++) {
        if (V_dist[u] == INFTY)
            continue;

        node_t *elem = E[u]->head;
        while (elem != NULL) {

            int v = elem->val;
            float alt = V_dist[u] + elem->w;

            // Se trovo un arco che rilassa ulteriormente la distanza (la quale dovrebbe essere
            // già con il valore minimo possibile), allora c'è un ciclo negativo
            if (alt < V_dist[v]) {
                printf("Errore: ciclo negativo raggiungibile dalla sorgente\n");
                return;
            }

            elem = elem->next;
        }
    }

}
