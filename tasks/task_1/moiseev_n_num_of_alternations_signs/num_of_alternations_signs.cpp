// Copyright 2023 Moiseev Nikita
#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include "task_1/moiseev_n_num_of_alternations_signs/num_of_alternations_signs.h"

std::vector<int> create_random_array(size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    const int low = -9'999;
    const int up = 9'999;
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
            if ((arr[i] > 0 && arr[i + 1] < 0) || (arr[i] < 0 && arr[i + 1] > 0)) {
                num_alternations++;
            }
        }
    }
    return num_alternations;
}

int par_num_of_alternations(const std::vector<int>& arr) {
    int size_world, rank;
    size_t size_arr = arr.size();
    if (size_arr < 2) return 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr_copy;
    if (rank == 0) {
        arr_copy = arr;
    }
    MPI_Bcast(&size_arr, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    arr_copy.resize(size_arr);
    MPI_Bcast(arr_copy.data(), static_cast<int>(size_arr), MPI_INT, 0, MPI_COMM_WORLD);
    int local_count = 0;
    for (size_t i = rank; i < size_arr - 1; i += size_world) {
        if ((arr_copy[i] > 0 && arr_copy[i + 1] < 0) || (arr_copy[i] < 0 && arr_copy[i + 1] > 0)) {
            local_count++;
        }
    }
    int global_count = 0;
    MPI_Allreduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    return global_count;
}
