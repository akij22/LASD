#include <sstream>

#include "csv_loader.h"

std::ifstream open_distances_file(const char** filename) {
    std::ifstream file(*filename);
    if (!file) {
        *filename = "Consegna5/distanze_linea_aria_capoluoghi_con_id.csv";
        file.open(*filename);
    }
    return file;
}

float** allocate_distance_matrix(int N) {
    // allocazione matrice 2D dinamica
    float** dist = new float*[N + 1];
    for (int i = 0; i <= N; ++i) {
        dist[i] = new float[N + 1];
        for (int j = 0; j <= N; ++j)
            dist[i][j] = 0.0f;
    }
    return dist;
}

void deallocate_distance_matrix(float** dist, int N) {
    for (int i = 0; i <= N; ++i)
        delete[] dist[i];
    delete[] dist;
}

void load_distances(std::ifstream& file, float** dist, std::string* city, arco* edges, int* n_edges) {
    std::string line;
    std::getline(file, line); // salta intestazione

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;

        int origine_id = 0;
        int destinazione_id = 0;
        float km = 0.0f;

        std::getline(ss, field, ',');
        origine_id = std::stoi(field);

        std::getline(ss, field, ',');
        destinazione_id = std::stoi(field);

        std::getline(ss, field, ',');
        city[origine_id] = field;

        std::getline(ss, field, ',');
        city[destinazione_id] = field;

        // distanza km
        std::getline(ss, field, ',');
        km = std::stof(field);

        dist[origine_id][destinazione_id] = km;
        dist[destinazione_id][origine_id] = km;

        edges[*n_edges].u = origine_id;
        edges[*n_edges].v = destinazione_id;
        edges[*n_edges].w = km;
        (*n_edges)++;
    }
}
