// Copyright 2023 Salaev Vladislav
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_1/salaev_v_max_of_matrix_rows/max_of_matrix_rows.h"

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

std::vector<size_t> getParallelMaxInRows(
    std::vector<int> glob_matr, int m, int n) {
    boost::mpi::communicator world;
    std::vector<size_t> glob_max_indexes(m);
    const int delta = m / world.size();
    const int remain = m % world.size();
    int rank = world.rank();
    std::vector<int> local_vecs(delta * n);
    std::vector<size_t> loc_max_indexes(delta);
    std::vector<int> sizes_i(world.size(), delta * n);
    std::vector<int> sizes(world.size(), delta);
    sizes[0] = delta + remain;
    sizes_i[0] = (delta + remain) * n;

    if (delta != 0)
        boost::mpi::scatterv(
            world, glob_matr.data(), sizes_i, local_vecs.data(), 0);

    if (rank == 0) {
        local_vecs = glob_matr;
        local_vecs.resize(sizes_i[0]);
        loc_max_indexes.resize(sizes[0]);
    }
    std::vector<int>::iterator row_max;
    for (int i = 0; i < sizes[rank]; ++i) {
        row_max = std::max_element(
            local_vecs.begin() + i * n, local_vecs.begin() + (i + 1) * n);
        size_t local_max_index = std::distance(
            local_vecs.begin(), row_max) - i * n;
        loc_max_indexes[i] = local_max_index + (
            rank == 0 ? 0 : delta * rank + remain) * n + i * n;
    }

    boost::mpi::gatherv(
        world, loc_max_indexes, glob_max_indexes.data(), sizes, 0);
    return glob_max_indexes;
}

std::vector<size_t> getSequentialMaxInRows(std::vector<int> matr, int m) {
    const int n = matr.size() / m;
    std::vector<size_t> glob_max_indexes(m);
    for (int i = 0; i < m; i++) {
        auto row_max = std::max_element(
            matr.begin() + i * n, matr.begin() + (i + 1) * n);
        glob_max_indexes[i] = std::distance(matr.begin(), row_max);
    }
    return glob_max_indexes;
}
