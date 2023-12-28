// Copyright 2023 Bakhtiarov Alexander
#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include "task_1/bakhtiarov_a_max_matrix_element/max_matrix_element.h"
std::vector<int> generate_random_Matrix(int rows, int cols) {
    if ((rows <= 0) || (cols <= 0)) {
        throw std::invalid_argument("Should be greater than zero");
    }
    std::mt19937 gen(static_cast<unsigned int>(time(0)));
    std::vector<int> matrix(rows * cols);
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = gen() % 10;
    }
    return matrix;
}
int findMaxOfMatrix(const std::vector<int>& global_matrix) {
    int max = std::numeric_limits<int>::min();
    for (const auto& element : global_matrix) {
        max = std::max(max, element);
    }
    return max;
}
int findMaxOfMatrixParallel(const std::vector<int>& global_matrix,
    int rows, int cols) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int elem_per_process = (rows * cols) / size;
    const int remainder = (rows * cols) % size;
    std::vector<int> send_counts(size, elem_per_process);
    std::vector<int> displs(size, 0);
    for (int i = 0; i < remainder; ++i) {
        send_counts[i]++;
    }
    for (int i = 1; i < size; ++i) {
        displs[i] = displs[i - 1] + send_counts[i - 1];
    }
    std::vector<int> local_matrix(send_counts[rank]);
    MPI_Scatterv(global_matrix.data(), send_counts.data(),
        displs.data(), MPI_INT, local_matrix.data(),
        send_counts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    int local_max = *std::max_element(local_matrix.begin(),
        local_matrix.end());
    int global_max = 0;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0,
        MPI_COMM_WORLD);
    return global_max;
}
