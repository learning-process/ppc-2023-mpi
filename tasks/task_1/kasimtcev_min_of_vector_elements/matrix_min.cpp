// Copyright 2023 Kasimtcev Roman
#include "task_1/kasimtcev_min_of_vector_elements/matrix_min.h"

std::vector<int> CreateRandomMatrix(int rows, int cols) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(rows * cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i * cols + j] = gen() % 123;
        }
    }
    return matrix;
}

int FndMin(std::vector<int> global_matrix) {
    int min = INT_MAX;
    for (int i = 0; i < global_matrix.size(); ++i) {
       min = std::min(global_matrix[i], min);
    }
    return min;
}

int FndMinParallel(std::vector<int> global_matrix) {
    int proc_num;
    int proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int matrix_size = global_matrix.size();
    const int chunk = matrix_size / proc_num;
    const int rem = matrix_size % proc_num;

    std::vector<int> send_counts(proc_num);
    std::vector<int> displs(proc_num);

    for (int i = 0; i < proc_num - 1; ++i) {
        send_counts[i] = chunk;
        displs[i] = i * chunk;
    }

    send_counts[proc_num-1] = (rem == 0) ? chunk :
       (matrix_size - (proc_num - 1) * chunk);
    displs[proc_num - 1] = (proc_num - 1) * chunk;

    std::vector<int> local_matrix(send_counts[proc_rank]);
    MPI_Scatterv(global_matrix.data(), send_counts.data(), displs.data(),
       MPI_INT, local_matrix.data(), send_counts[proc_rank], MPI_INT, 0,
       MPI_COMM_WORLD);

    int global_min = INT_MAX;
    int local_min = FndMin(local_matrix);
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    return global_min;
}
