#include <iostream>
#include <string>

#include "csv_loader.h"
#include "kruskal.h"

/// apre il file CSV e carica le distanze in una matrice 2D dinamica, poi stampa un esempio di accesso alla matrice

// obiettivo: implementare kruskal e identificare gli ultimi archi aggiunti
// descrivere le coppie di città che corrispondono agli ultimi archi aggiunti e il significato geografico di queste coppie

int main() {
    const char* filename = "distanze_linea_aria_capoluoghi_con_id.csv";

    std::ifstream file = open_distances_file(&filename);
    if (!file) {
        std::cerr << "Errore apertura file\n";
        return 1;
    }

    const int N = 107; // numero capoluoghi

    float** dist = allocate_distance_matrix(N);

    std::string* city = new std::string[N + 1];
    arco* edges = new arco[N * (N - 1) / 2];
    int n_edges = 0;

    load_distances(file, dist, city, edges, &n_edges);

    // esempio accesso
    std::cout << "Distanza 1 -> 2: " << dist[1][2] << " km\n";

    kruskal_clustering(edges, n_edges, city, N);

    // deallocazione
    deallocate_distance_matrix(dist, N);
    delete[] city;
    delete[] edges;

    return 0;
}
