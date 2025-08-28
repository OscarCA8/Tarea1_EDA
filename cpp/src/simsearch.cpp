#include "simsearch.hpp"
#include "utils.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

SimSearch::SimSearch(const Matrix &data, const Matrix &clusters)
    : mat_data(data), mat_clusters(clusters) {}

// búsqueda sin clusters
std::vector<size_t> SimSearch::search(const float *query, size_t top_k) {
    size_t n = mat_data.getN();
    size_t dim = mat_data.getDim();

    std::vector<float> dist(n);
    for (size_t i = 0; i < n; i++) {
        const float *row = mat_data.getRow(i);
        dist[i] = vec_compute_distance(query, row, dim);
    }

    size_t *indices = argsort(dist.data(), n);

    std::vector<size_t> result;
    for (size_t i = 0; i < top_k; i++) {
        result.push_back(indices[i]);
    }

    delete[] indices;
    return result;
}

// búsqueda con clusters (incompleta por ahora)
std::vector<size_t> SimSearch::search_with_clusters(const float *query, size_t top_k) {
    size_t k = mat_clusters.getN();  // número de centroides
    size_t dim = mat_clusters.getDim();

    // calcular distancia a cada centroide
    std::vector<float> dist_c(k);
    for (size_t j = 0; j < k; j++) {
        const float *centroid = mat_clusters.getRow(j);
        dist_c[j] = vec_compute_distance(query, centroid, dim);
    }

    // escoger el cluster más cercano
    size_t *sorted_c = argsort(dist_c.data(), k);
    size_t best_cluster = sorted_c[0];
    delete[] sorted_c;

    // 🔴 Por ahora no tenemos acceso a los índices de los puntos por cluster
    // porque esos están dentro de la clase Cluster, no en SimSearch.
    // Entonces aquí deberíamos integrar con Cluster::getInds(best_cluster).

    std::vector<size_t> result;
    // TODO: implementar cuando integremos con Cluster
    return result;
}
