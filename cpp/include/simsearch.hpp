#ifndef _SIM_SEARCH_HPP
#define _SIM_SEARCH_HPP
#include <vector>
#include "matrix.hpp"
#include "cluster.hpp"
class SimSearch{
private:
    const Matrix &mat_data; // data to be processed
    const Matrix &mat_clusters; // centroids to be used
    const Cluster &cluster;

public:
    SimSearch(const Matrix &data, const Matrix &clusters);
    std::vector<size_t> search_with_clusters(const float *query,  size_t top_k);
    std::vector<size_t> search(const float *query, size_t top_k);
};

#endif 
