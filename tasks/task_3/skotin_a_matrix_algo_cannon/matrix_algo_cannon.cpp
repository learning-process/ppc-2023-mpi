// Copyright 2023 Skotin Alexander
#include "task_3/skotin_a_matrix_algo_cannon/matrix_algo_cannon.h"

std::vector<std::vector<double>> GetMatrixRandom(int size) {
    std::vector<std::vector<double>> matrix(size, std::vector<double>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = static_cast<double>(
                rand()) / RAND_MAX * 100.0 - 50.0;
        }
    }
    return matrix;
}

std::vector<std::vector<double>> MatrixMultiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, int matrix_dimension) {
    std::vector<std::vector<double>> result(matrix_dimension,
        std::vector<double>(matrix_dimension, 0));
    for (int i = 0; i < matrix_dimension; i++) {
        for (int j = 0; j < matrix_dimension; j++) {
            for (int k = 0; k < matrix_dimension; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}
std::vector<std::vector<double>> CannonMultiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, int matrix_dimension) {
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int sqrt_proc_num = static_cast<int>(sqrt(world_size));
    int local_size = matrix_dimension / sqrt_proc_num;
    int local_matrix_size = local_size * local_size;
    std::vector<double> local_A(local_matrix_size), local_B(local_matrix_size),
        local_C(local_matrix_size, 0.0);
    std::vector<double> flat_A(matrix_dimension * matrix_dimension),
        flat_B(matrix_dimension * matrix_dimension);
    if (world_rank == 0) {
        for (int i = 0; i < matrix_dimension; i++) {
            for (int j = 0; j < matrix_dimension; j++) {
                flat_A[i * matrix_dimension + j] = A[i][j];
                flat_B[i * matrix_dimension + j] = B[i][j];
            }
        }
    }
    MPI_Scatter(flat_A.data(), local_matrix_size, MPI_DOUBLE, local_A.data(),
        local_matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(flat_B.data(), local_matrix_size, MPI_DOUBLE, local_B.data(),
        local_matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for (int i = 0; i < local_size; i++) {
        for (int j = 0; j < local_size; j++) {
            for (int k = 0; k < local_size; k++) {
                local_C[i * local_size + j] += local_A[i * local_size + k]
                    * local_B[k * local_size + j];
            }
        }
    }
    std::vector<double> gathered_C(matrix_dimension * matrix_dimension);
    MPI_Gather(local_C.data(), local_matrix_size, MPI_DOUBLE, gathered_C.data(),
        local_matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::vector<std::vector<double>> result(matrix_dimension,
        std::vector<double>(matrix_dimension));
    if (world_rank == 0) {
        for (int i = 0; i < matrix_dimension; i++) {
            for (int j = 0; j < matrix_dimension; j++) {
                result[i][j] = gathered_C[i * matrix_dimension + j];
            }
        }
    }
    return result;
}
