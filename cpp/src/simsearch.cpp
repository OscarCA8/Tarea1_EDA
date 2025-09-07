#include "simsearch.hpp"
#include "utils.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

SimSearch::SimSearch(const Matrix &data, const Matrix &clusters, const Cluster &clust)
    : mat_data(data), mat_clusters(clusters), cluster(clust) {}

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

std::vector<size_t> SimSearch::search_with_clusters(const float *query, size_t top_k) {
    size_t k = mat_clusters.getN();   // number of clusters
    size_t dim = mat_clusters.getDim();

    std::vector<float> dist_c(k);
    for (size_t j = 0; j < k; j++) {
        const float *centroid = mat_clusters.getRow(j);
        dist_c[j] = vec_compute_distance(query, centroid, dim);
    }

    size_t *sorted_c = argsort(dist_c.data(), k);

    std::vector<size_t> candidates; 
    size_t needed = top_k;
    size_t idx_cluster = 0;

    while (needed > 0 && idx_cluster < k) {
        size_t c_id = sorted_c[idx_cluster];
        std::vector<size_t> inds = cluster.getInds(c_id);

        for (size_t ind : inds) {
            candidates.push_back(ind);
        }

        needed = (candidates.size() >= top_k) ? 0 : top_k - candidates.size();
        idx_cluster++;
    }

    delete[] sorted_c;

    std::vector<float> dist_cand(candidates.size());
    for (size_t i = 0; i < candidates.size(); i++) {
        const float *row = mat_data.getRow(candidates[i]);
        dist_cand[i] = vec_compute_distance(query, row, mat_data.getDim());
    }

    size_t *indices = argsort(dist_cand.data(), candidates.size());

    std::vector<size_t> result;
    for (size_t i = 0; i < top_k && i < candidates.size(); i++) {
        result.push_back(candidates[indices[i]]);
    }

    delete[] indices;
    return result;
}
