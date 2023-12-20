// Copyright 2023 Ryabkov Vladislav

#include "task_2/ryabkov_v_vertical_strip/strip_vertical_scheme.h"
#include <algorithm>
#include <mpi.h>
#include <random>
#include <vector>

int* init_empty_matrix(int rows) {
    int* matrix = new int[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = 0;
    }
    return matrix;
}

int* get_random_matrix(int rows, int columns) {
    int* matrix = new int[rows * columns];
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < rows * columns; i++) {
        matrix[i] = gen() % 20;
    }
    return matrix;
}

void sequential_matrix_multiplication(const int* A, const int A_rows,
    const int A_columns, const int* B,
    const int B_rows, int* res) {
    for (int i = 0; i < A_rows; i++) {
        res[i] = 0;
        int tmp = 0;
        for (int k = 0; k < A_columns; k++) {
            tmp += A[i * A_columns + k] * B[k];
            res[i] = tmp;
        }
    }
}

void parallel_matrix_multiplication(const int* A, const int A_rows,
    const int A_columns, const int* B,
    const int B_rows, int* res) {
    if (A_columns != B_rows) {
        throw "ERROR";
    }
    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int strip_width = A_columns / world_size;
    const int remainder = A_columns % world_size;
    int* strip = nullptr, * local_vector = nullptr;
    int new_width = strip_width + remainder;
    if (rank == 0) {
        strip = new int[A_rows * new_width];
    }
    else {
        strip = new int[A_rows * strip_width];
    }
    local_vector = init_empty_matrix(A_rows);
    if (rank == 0) {
        for (int i = 0; i < A_rows; i++) {
            MPI_Scatter(A + remainder + i * A_columns, strip_width, MPI_INT,
                strip + remainder + i * new_width, strip_width, MPI_INT, 0,
                MPI_COMM_WORLD);
            for (int j = 0; j < remainder; j++) {
                strip[j + i * new_width] = A[j + i * A_columns];
            }
        }
    }
    else {
        for (int i = 0; i < A_rows; i++) {
            MPI_Scatter(A + remainder + i * A_columns, strip_width, MPI_INT,
                strip + i * strip_width, strip_width, MPI_INT, 0,
                MPI_COMM_WORLD);
        }
    }
    if (rank == 0) {
        sequential_matrix_multiplication(strip, A_rows, new_width, B, new_width,
            local_vector);
    }
    else {
        int* new_B = new int[strip_width];
        std::copy(B + strip_width * rank + remainder,
            B + strip_width * rank + remainder + strip_width, new_B);
        sequential_matrix_multiplication(strip, A_rows, strip_width, new_B,
            strip_width, local_vector);
    }
    MPI_Reduce(local_vector, res, A_rows, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
}
