#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include <fstream>
#include <string>

#include "data_types.h"

std::ifstream open_distances_file(const char** filename);
float** allocate_distance_matrix(int N);
void deallocate_distance_matrix(float** dist, int N);
void load_distances(std::ifstream& file, float** dist, std::string* city, arco* edges, int* n_edges);

#endif
