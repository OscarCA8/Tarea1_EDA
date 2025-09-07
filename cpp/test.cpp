#include "utils.hpp"
#include "matrix.hpp"
#include "simsearch.hpp"
#include <iostream>
#include <chrono>
#include <vector>
using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    string str_data    = "/home/pbn/Documents/EDA/data_eda.npy";
    string str_queries = "/home/pbn/Documents/EDA/queries_eda.npy";

    Matrix mat(str_data);     // catálogo X (n=1000)
    Matrix queries(str_queries); // consultas Q (100)

    // SimSearch (por ahora no usamos clusters)
    SimSearch sim(mat, mat); // segundo parámetro dummy (no usado aún)

    size_t m = 8;
    size_t num_queries = queries.getN();

    long long total_time_dist = 0;
    long long total_time_sort = 0;
    size_t total_comparisons = 0;

    for (size_t i = 0; i < num_queries; i++) {
        const float *q = queries.getRow(i);

        // medir tiempo de distancias
        auto start_dist = high_resolution_clock::now();
        std::vector<float> dist(mat.getN());
        for (size_t j = 0; j < mat.getN(); j++) {
            const float *row = mat.getRow(j);
            dist[j] = vec_compute_distance(q, row, mat.getDim());
        }
        auto end_dist = high_resolution_clock::now();
        total_time_dist += duration_cast<microseconds>(end_dist - start_dist).count();

        // comparaciones = n por consulta
        total_comparisons += mat.getN();

        // medir tiempo de ordenamiento
        auto start_sort = high_resolution_clock::now();
        size_t *indices = argsort(dist.data(), mat.getN());
        auto end_sort = high_resolution_clock::now();
        total_time_sort += duration_cast<microseconds>(end_sort - start_sort).count();

        delete[] indices;
    }

    cout << "Resultados búsqueda normal (sin clusters)" << endl;
    cout << "Comparaciones promedio por consulta: " << (total_comparisons / num_queries) << endl;
    cout << "Tiempo promedio distancias (us)     : " << (total_time_dist / num_queries) << endl;
    cout << "Tiempo promedio ordenar (us)        : " << (total_time_sort / num_queries) << endl;

    return 0;
}
