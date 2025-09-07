#include "utils.hpp"
#include "matrix.hpp"
#include "cluster.hpp"
#include "simsearch.hpp"

#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_set>

using namespace std;
using namespace std::chrono;

void run_experiment_k(const Matrix &mat, const Matrix &queries) {
    cout << "=== Experimento 1: variación de k con m=8 ===" << endl;
    cout << "=== Para mayor simplicidad, hemos puesto el caso sin cluster en el experimento 2==="<< endl;
    vector<size_t> k_values = {8, 16, 32, 64, 128};
    size_t m = 8;
    size_t num_queries = queries.getN();

    for (size_t k : k_values) {
        Cluster cluster(mat, k);
        cluster.applyClustering();
        SimSearch sim(mat, cluster.getCentroidsMatrix(), cluster);

        long long total_time = 0;
        size_t total_comparisons = 0;

        for (size_t i = 0; i < num_queries; i++) {
            const float *q = queries.getRow(i);

            auto start = high_resolution_clock::now();
            vector<size_t> res = sim.search_with_clusters(q, m);
            auto end = high_resolution_clock::now();
            total_time += duration_cast<microseconds>(end - start).count();

            total_comparisons += res.size(); 
        }

        cout << "k = " << k << endl;
        cout << "Comparaciones promedio por consulta: " << (total_comparisons / num_queries) << endl;
        cout << "Tiempo promedio (us): " << (total_time / num_queries) << endl;
        cout << "--------------------------------------" << endl;
    }
}

void run_experiment_m_no_clusters(const Matrix &mat, const Matrix &queries) {
    cout << "\n=== Experimento 2: variación de m sin clusters ===" << endl;
    vector<size_t> m_values = {8, 16, 32, 64, 128};
    size_t num_queries = queries.getN();

    for (size_t m : m_values) {
        long long total_time_dist = 0;
        long long total_time_sort = 0;
        size_t total_comparisons = 0;

        for (size_t i = 0; i < num_queries; i++) {
            const float *q = queries.getRow(i);

            auto start_dist = high_resolution_clock::now();
            vector<float> dist(mat.getN());
            for (size_t j = 0; j < mat.getN(); j++) {
                const float *row = mat.getRow(j);
                dist[j] = vec_compute_distance(q, row, mat.getDim());
            }
            auto end_dist = high_resolution_clock::now();
            total_time_dist += duration_cast<microseconds>(end_dist - start_dist).count();

            total_comparisons += mat.getN();

            auto start_sort = high_resolution_clock::now();
            size_t *indices = argsort(dist.data(), mat.getN());
            auto end_sort = high_resolution_clock::now();
            total_time_sort += duration_cast<microseconds>(end_sort - start_sort).count();

            delete[] indices;
        }

        cout << "m = " << m << endl;
        cout << "Comparaciones promedio por consulta: " << (total_comparisons / num_queries) << endl;
        cout << "Tiempo promedio distancias (us): " << (total_time_dist / num_queries) << endl;
        cout << "Tiempo promedio ordenar (us):    " << (total_time_sort / num_queries) << endl;
        cout << "--------------------------------------" << endl;
    }
}

void run_experiment_m_with_clusters(const Matrix &mat, const Matrix &queries) {
    cout << "\n=== Experimento 3: variación de m con clusters ===" << endl;
    vector<size_t> k_values = {8, 16, 32, 64, 128};
    vector<size_t> m_values = {16, 32, 64, 128};
    size_t num_queries = queries.getN();

    for (size_t k : k_values) {
        Cluster cluster(mat, k);
        cluster.applyClustering();
        SimSearch sim(mat, cluster.getCentroidsMatrix(), cluster);

        cout << "\n--- k = " << k << " ---" << endl;

        for (size_t m : m_values) {
            long long total_time = 0;
            size_t total_comparisons = 0;

            for (size_t i = 0; i < num_queries; i++) {
                const float *q = queries.getRow(i);

                auto start = high_resolution_clock::now();
                vector<size_t> res = sim.search_with_clusters(q, m);
                auto end = high_resolution_clock::now();
                total_time += duration_cast<microseconds>(end - start).count();

                total_comparisons += res.size();
            }

            cout << "m = " << m << endl;
            cout << "Comparaciones promedio por consulta: " << (total_comparisons / num_queries) << endl;
            cout << "Tiempo promedio (us): " << (total_time / num_queries) << endl;
            cout << "--------------------------------------" << endl;
        }
    }
}

void run_error_analysis(const Matrix &mat, const Matrix &queries) {
    cout << "\n=== Experimento 4: error de búsqueda ===" << endl;
    vector<size_t> k_values = {8, 16, 32, 64, 128};
    vector<size_t> m_values = {8, 16, 32, 64, 128};
    size_t num_queries = queries.getN();

    for (size_t k : k_values) {
        Cluster cluster(mat, k);
        cluster.applyClustering();
        SimSearch sim(mat, cluster.getCentroidsMatrix(), cluster);

        cout << "\n--- k = " << k << " ---" << endl;

        for (size_t m : m_values) {
            double total_recall = 0.0;

            for (size_t i = 0; i < num_queries; i++) {
                const float *q = queries.getRow(i);

                vector<size_t> res_true = sim.search(q, m);

                vector<size_t> res_cluster = sim.search_with_clusters(q, m);

                unordered_set<size_t> set_true(res_true.begin(), res_true.end());
                size_t inter = 0;
                for (size_t idx : res_cluster) {
                    if (set_true.count(idx)) inter++;
                }

                total_recall += double(inter) / double(m);
            }

            cout << "m = " << m << " -> error promedio: " << (total_recall / num_queries) << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    string str_data    = "/home/pbn/Documents/EDA/data_eda.npy";
    string str_queries = "/home/pbn/Documents/EDA/queries_eda.npy";

    Matrix mat(str_data);
    Matrix queries(str_queries);

    run_experiment_k(mat, queries);
    run_experiment_m_no_clusters(mat, queries);
    run_experiment_m_with_clusters(mat, queries);
    run_error_analysis(mat, queries);

    return 0;
}

