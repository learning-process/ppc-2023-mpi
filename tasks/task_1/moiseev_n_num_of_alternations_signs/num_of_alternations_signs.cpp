// Copyright 2023 Moiseev Nikita
#include <mpi.h>
#include <vector>
#include <iostream>
#include "task_1/moiseev_n_num_of_alternations_signs/num_of_alternations_signs.h"

std::vector<int> create_random_array(size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    const int low = -999;
    const int up = 999;
    std::uniform_int_distribution<int> dist(low, up);
    std::vector<int> arr(size);
    for (auto& elem : arr) elem = dist(gen);
    return arr;
}

int seq_num_of_alternations(const std::vector<int>& arr) {
    size_t size = arr.size();
    int num_alternations = 0;
    if (size >= 2) {
        for (size_t i = 0; i < size - 1; ++i) {
            if ((arr[i] * arr[i + 1]) < 0) {
                num_alternations++;
            }
        }
    }
    return num_alternations;
}

int par_num_of_alternations(const std::vector<int>& arr) {
    int size_world, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    size_t size_arr = arr.size();
    if (size_arr < 2) return 0;
    int block_size = size_arr / size_world;
    std::vector<int> counts_vector(size_world, block_size);
    std::vector<int> move_vector(size_world);
    int end = size_arr % size_world;
    for (int i = 0; i < end; ++i) {
        counts_vector[i] = block_size + 1;
    }
    move_vector[0] = 0;
    for (size_t i = 1; i < counts_vector.size(); ++i) {
        move_vector[i] = move_vector[i - 1] + counts_vector[i - 1];
    }
    for (int i = 0; i < size_world - 1; i++) {
        if (counts_vector[i + 1] == 0) break;
        counts_vector[i]++;
    }
    std::vector<int> arr_copy(counts_vector[rank]);
    MPI_Scatterv(arr.data(), counts_vector.data(), move_vector.data(), MPI_INT,
       arr_copy.data(), counts_vector[rank], MPI_INT, 0, MPI_COMM_WORLD);
    int local_alternations_count = seq_num_of_alternations(arr_copy);
    int global_alternations_count;
    MPI_Allreduce(&local_alternations_count, &global_alternations_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    return global_alternations_count;
}
