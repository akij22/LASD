#include <iostream>

#include "edge_sort.h"
#include "kruskal.h"
#include "union_find.h"

void print_cluster(int* parent, std::string* city, int N, int root) {
    int count = 0;
    for (int i = 1; i <= N; i++)
        if (find_set(parent, i) == root)
            count++;

    std::cout << "dimensione " << count << ": ";
    for (int i = 1; i <= N; i++) {
        if (find_set(parent, i) == root) {
            std::cout << city[i];
            count--;
            if (count > 0)
                std::cout << ", ";
        }
    }
    std::cout << "\n";
}

void kruskal_clustering(arco* edges, int n_edges, std::string* city, int N) {

    // Primo step = ordinamento degli archi per peso in modo crescente
    edge_quicksort(edges, 0, n_edges - 1);

    int* parent = new int[N + 1];
    int* rank = new int[N + 1];


    // All'inizio, ogni città rappresenta un cluster / insieme separato
    // Avrò un numero di insiemi pari al numero di capoluoghi
    for (int i = 1; i <= N; i++) {

        // Dato un nodo `i`, `parent[i]` rappresenta il nodo radice dell'albero in cui `i` è contenuto
        parent[i] = i;
        rank[i] = 0;
    }

    const int LAST_EDGES = 10;
    int accepted = 0;

    std::cout << "\nUltimi archi aggiunti da Kruskal:\n";

    // Termino quando trovo il MST t.c, dato |N| = numero di nodi, avrò |N| - 1 = numero di archi necessari per ottenere il MST
    for (int i = 0; i < n_edges && accepted < N - 1; i++) {


        // Dato l'arco corrente, ottengo i due insiemi a cui i nodi dell'arco appartengono
        int root_u = find_set(parent, edges[i].u);
        int root_v = find_set(parent, edges[i].v);


        // Se trovo due cluster separati proseguo
        // Altrimenti, significa che l'arco corrente creerebbe un ciclo nel MST e quindi lo ignoro
        if (root_u != root_v) {

            // Stampa degli ultimi 10 archi aggiunti all'MST
            if (accepted >= (N - 1) - LAST_EDGES) {
                std::cout << "\nArco " << accepted + 1 << " dell'MST: "
                          << city[edges[i].u] << " - " << city[edges[i].v]
                          << " (" << edges[i].w << " km)\n";

                std::cout << "Cluster A prima della fusione: ";
                print_cluster(parent, city, N, root_u);

                std::cout << "Cluster B prima della fusione: ";
                print_cluster(parent, city, N, root_v);
            }

            // Unisco due archi sotto un unico cluster, rappresentato da `parent`
            union_set(parent, rank, edges[i].u, edges[i].v);
            accepted++;
        }

        // std::cout << "Ho trovato un ciclo, lo ignoro...\n" << std::endl;
    }

    std::cout << "\nArchi totali nell'albero di copertura minimo: " << accepted << "\n";
    delete[] parent;
    delete[] rank;
}
