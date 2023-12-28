// Copyright 2023 Platonova Maria
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_1/platonova_m_min_matrix_rows/min_matrix_rows.h"

std::vector<int> getRandomMatrix(int m, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matr(m * n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matr[i * n + j] = gen() % 100;
        }
    }
    return matr;
}

// assume what m = delta * world.size();
std::vector<size_t> getParallelMin(std::vector<int> glob_matr, int m, int n) {
    boost::mpi::communicator world;
    std::vector<size_t> glob_min_indexes(m);
    const int delta = m / world.size();
    const int remain = m % world.size();
    int rank = world.rank();
    std::vector<int> local_vecs(delta * n);
    std::vector<size_t> loc_min_indexes(delta);
    std::vector<int> sizes_i(world.size(), delta * n);
    std::vector<int> sizes(world.size(), delta);
    sizes[0] = delta + remain;
    sizes_i[0] = (delta + remain) * n;

    if (delta != 0)
        boost::mpi::scatterv(world, glob_matr.data(),
            sizes_i, local_vecs.data(), 0);

    if (rank == 0) {
        local_vecs = glob_matr;
        local_vecs.resize(sizes_i[0]);
        loc_min_indexes.resize(sizes[0]);
    }
    std::vector<int>::iterator row_min;
    for (int i = 0; i < sizes[rank]; ++i) {
        row_min = std::min_element(local_vecs.begin() + i * n,
                                   local_vecs.begin() + (i + 1) * n);
        loc_min_indexes[i] = std::distance(local_vecs.begin(), row_min);
        if (rank != 0)
            loc_min_indexes[i] += (delta * rank + remain) * n;
    }
    boost::mpi::gatherv(world, loc_min_indexes,
        glob_min_indexes.data(), sizes, 0);
    return glob_min_indexes;
}

std::vector<size_t> getSequentialMin(std::vector<int> matr, int m) {
    const int n = matr.size() / m;
    std::vector<int>::iterator row_min;
    std::vector<size_t> glob_min_indexes(m);
    for (int i = 0; i < m; i++) {
        row_min = std::min_element(matr.begin() + i * n,
                                   matr.begin() + (i + 1) * n);
        glob_min_indexes[i] = std::distance(matr.begin(), row_min);
    }
    return glob_min_indexes;
}
