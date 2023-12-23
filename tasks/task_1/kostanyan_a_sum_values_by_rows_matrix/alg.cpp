// Copyright 2023 Kostanyan Arsen
#include "task_1/kostanyan_a_sum_values_by_rows_matrix/alg.h"


std::vector<int> RandomMatrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw - 1;
    }
    std::random_device dev;
    std::mt19937 gen(dev());

    std::vector<int> matrix(rows * cols);
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = gen() % 100;
    }
    return matrix;
}

std::vector<int> calculateRowSums(const std::vector<int>& matrix, int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Invalid matrix");
    }
    std::vector<int> sum(rows);
    for (int i = 0; i < rows; i++) {
        sum[i] = 0;
        for (int j = 0; j < cols; j++) {
            sum[i] += matrix[i * cols + j];
        }
    }
    return sum;
}

std::vector<int> ParallelProcessing(const std::vector<int>& matrix, int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Invalid matrix");
    }
    int size, rank;
    std::vector<int> buffer;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rows < size) {
        throw std::invalid_argument("Number of processes exceeds matrix rows.");
    }
    const int delta = rows % size;
    const int buffer_size = rows / size;

    buffer.resize(cols * buffer_size);
    MPI_Scatter(matrix.data(), cols * buffer_size, MPI_INT,
        buffer.data(), cols * buffer_size, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> local_sum = calculateRowSums(buffer, buffer_size, cols);

    std::vector<int> global_sum;
    if (rank == 0) {
        global_sum.resize(rows);
        for (int i = 0; i < rows; i++) {
            global_sum[i] = 0;
        }
    }

    MPI_Gather(local_sum.data(), buffer_size, MPI_INT,
        global_sum.data(), buffer_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < delta; i++) {
            for (int j = 0; j < cols; j++) {
                global_sum[buffer_size * size + i] +=
                    matrix[(buffer_size * size + i) * cols + j];
            }
        }
    }

    return global_sum;
}
