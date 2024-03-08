// Copyright 2023 Khavronina Alexandra

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_1/khavronina_a_min_matrix/min_matrix.h"

std::vector<int> gen_random_matrix(int m, int n) {
    std::random_device rand_dev;
    std::mt19937 gen(rand_dev());
    std::vector<int> matrix(m * n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[n * i + j] = gen() % 100;
        }
    }
    return matrix;
}

std::vector<size_t> find_min_values_in_matrix_rows_parallel(std::vector<int> global_matrix, int m, int n) {
    boost::mpi::communicator world;
    std::vector<size_t> min_indexes_global(m);
    const int rows_for_process = m / world.size();
    const int remaining_rows = m % world.size();
    int rank = world.rank();
    std::vector<int> local_data_for_process(rows_for_process * n);
    std::vector<size_t> local_min_index(rows_for_process);

    std::vector<int> sizes_for_scatterv(world.size(), rows_for_process * n);
    std::vector<int> sizes_for_gatherv(world.size(), rows_for_process);
    sizes_for_gatherv[0] = rows_for_process + remaining_rows;
    sizes_for_scatterv[0] = (rows_for_process + remaining_rows) * n;

    if (rows_for_process != 0)
        boost::mpi::scatterv(world, global_matrix.data(), sizes_for_scatterv, local_data_for_process.data(), 0);

    if (rank == 0) {
        local_data_for_process = global_matrix;
        local_data_for_process.resize(sizes_for_scatterv[0]);
        local_min_index.resize(sizes_for_gatherv[0]);
    }

    std::vector<int>::iterator iterator_min_el;

    for (int i = 0; i < sizes_for_gatherv[rank]; i++) {
        iterator_min_el = std::min_element(local_data_for_process.begin() + i * n,
                                   local_data_for_process.begin() + (i + 1) * n);
        local_min_index[i] = std::distance(local_data_for_process.begin(), iterator_min_el);

        if (rank != 0)
            local_min_index[i] += (rows_for_process * rank + remaining_rows) * n;
    }

    boost::mpi::gatherv(world, local_min_index, min_indexes_global.data(), sizes_for_gatherv, 0);

    return min_indexes_global;
}

std::vector<size_t> get_min_values_in_matrix_rows(std::vector<int> matrix, int m) {
    const int n = matrix.size() / m;
    std::vector<int>::iterator iterator_min_el;
    std::vector<size_t> min_indexes_global(m);

    for (int i = 0; i < m; i++) {
        iterator_min_el = std::min_element(matrix.begin() + i * n, matrix.begin() + (i + 1) * n);
        min_indexes_global[i] = std::distance(matrix.begin(), iterator_min_el);
    }

    return min_indexes_global;
}
