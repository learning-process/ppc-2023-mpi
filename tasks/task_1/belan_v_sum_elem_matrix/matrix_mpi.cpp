/* Copyright 2023 Belan Vadim */

#include <mpi.h>
#include <iostream>
#include <algorithm>
#include "task_1/belan_v_sum_elem_matrix/matrix_mpi.h"

int calcMatrixSum(int** matrix, int size, int world_rank, int world_size) {
    int sum = 0;

    int rows_per_proc = (size + world_size - 1) / world_size;
    int start_row = world_rank * rows_per_proc;
    int end_row = std::min(start_row + rows_per_proc, size);

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < size; ++j) {
            sum += matrix[i][j];
        }
    }

    int total_sum = 0;
    MPI_Allreduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    return total_sum;
}

void deleteMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
