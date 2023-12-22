// Copyright 2023 Bodrov Daniil
#include "task_1/bodrov_d_max_row_values/max_row_values.h"

#include <algorithm>

std::vector<int> FindMaxRowValues(const std::vector<int>& matrix, int n) {
    std::vector<int> max_values(n);
    for (int i = 0; i < n; ++i) {
        max_values[i] = *std::max_element(matrix.begin() + i * n, matrix.begin() + (i + 1) * n);
    }
    return max_values;
}

std::vector<int> FindMaxRowValuesPar(const std::vector<int>& matrix, int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int rows_per_proc = n / size;
    int remaining_rows = n % size;

    int start_row = rank * rows_per_proc + std::min(rank, remaining_rows);
    int end_row = start_row + rows_per_proc + (rank < remaining_rows);

    std::vector<int> local_max_values(end_row - start_row);
    for (int i = start_row; i < end_row; ++i) {
        local_max_values[i - start_row] = *std::max_element(matrix.begin() + i * n, matrix.begin() + (i + 1) * n);
    }

    if (rank == 0) {
        for (int proc = 1; proc < size; ++proc) {
            start_row = proc * rows_per_proc + std::min(proc, remaining_rows);
            end_row = start_row + rows_per_proc + (proc < remaining_rows);
            std::vector<int> proc_max_values(end_row - start_row);
            MPI_Recv(proc_max_values.data(), end_row - start_row, MPI_INT, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_max_values.insert(local_max_values.end(), proc_max_values.begin(), proc_max_values.end());
        }
    } else {
        MPI_Send(local_max_values.data(), end_row - start_row, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return local_max_values;
}
