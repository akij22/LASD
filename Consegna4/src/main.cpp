#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/algorithms.h"
#include "../include/cli.h"
#include "../include/graph.h"
#include "../include/list.h"
#include "../include/state.h"

// compilazione: g++ lezione17-grafi-dijkstra.cpp
//
// Obiettivo:
// 1) grafo con archi pesati
// 2) implementazione shortest path

int main(int argc, char **argv) {
    int i, test;

    // init random
    srand((unsigned)time(NULL));

    if (parse_cmd(argc, argv))
        return 1;

    int N = 10;
    n_nodi = N * N;

    n_nodi = 10;

    //// init nodi
    V = new int[n_nodi];
    V_visitato = new int[n_nodi];
    V_prev = new int[n_nodi];
    V_dist = new float[n_nodi];
    heap = new int[n_nodi];
    heap_pos = new int[n_nodi];

    //// init archi
    E = new list_t *[n_nodi]; //(list_t**)malloc(n_nodi*sizeof(list_t*));

    // costruzione grafo
    for (int i = 0; i < n_nodi; i++) {
        V[i] = 2 * i;
        V_visitato[i] = 0; // flag = non visitato
        V_prev[i] = -1;    // non c'e' precedente
        V_dist[i] = INFTY; // infinito

        E[i] = list_new();

        if (i == 0)
            global_ptr_ref = E[i];

        int x = i % N;
        int y = i / N;

        // for (int dx = -2; dx <= 2; dx += 1)
        //     for (int dy = -2; dy <= 2; dy += 1)
        //         if (abs(dx) + abs(dy) >= 1 &&
        //             abs(dx) + abs(dy) <= 1
        //             ) { // limito gli archi ai vicini con 1 variazione assoluta sulle coordinate

        //             int nx = x + dx;
        //             int ny = y + dy;

        //             if (nx >= 0 && nx < N &&
        //                 ny >= 0 && ny < N) { /// coordinate del nuovo nodo sono nel grafo

        //                 int j = nx + N * ny; /// indice del nuovo nodo
        //                 list_insert_front(E[i], j, 15 * sqrt(dx*dx + dy*dy));
        //             }
        //         }
    }

    int partenza = 0;
    int arrivo = n_nodi - 1;
    int w_max = 100;

    for (int i = 0; i < n_nodi - 1; i++) {
        /// arco costoso
        list_insert_front(E[i], arrivo, w_max - 2 * i);
        /// arco 1
        if (i > 0)
            list_insert_front(E[i-1], i , 1);
    }

    // for (int i = 0; i < n_nodi - 1; i++) {
    //     for (int j = 0; j < n_nodi - 1; j++) {
    //         /// arco costoso
    //     // list_insert_front(E[i], arrivo, w_max - 2 * i);
    //     /// arco 1
    //     if (i<j)
    //         list_insert_front(E[i], j , i+j);
    //     }
    // }

    for (int i = 0; i < n_nodi; i++) {
        printf("Sono il nodo di indice %d nell'array\n", i);
        printf("Il valore del nodo e' %d\n", V[i]);
        printf("La lista di adiacenza e'\n");
        list_print(E[i]);
    }

    shortest_path(0);

    // DA CAPIRE QUESTA FUNZIONE
    reset_shortest_path_data();
    bellman_ford(0);
    reset_shortest_path_data();

    printf("\nEsempio Bellman-Ford con un arco negativo ma senza ciclo negativo\n");
    init_empty_graph(4);
    list_insert_front(E[0], 1, 4);
    list_insert_front(E[0], 2, 5);
    list_insert_front(E[1], 2, -2);
    list_insert_front(E[2], 3, 3);
    bellman_ford(0);

    for (int i = 0; i < n_nodi; i++)
        printf("Nodo %d: distanza %.2f, precedente %d\n", i, V_dist[i], V_prev[i]);

    printf("\nEsempio Bellman-Ford con ciclo negativo raggiungibile\n");
    init_empty_graph(3);
    list_insert_front(E[0], 1, 1);
    list_insert_front(E[1], 2, -2);
    list_insert_front(E[2], 0, -2);
    bellman_ford(0);
    for (int i = 0; i < n_nodi; i++)
        printf("Nodo %d: distanza %.2f, precedente %d\n", i, V_dist[i], V_prev[i]);

    return 0;
}
