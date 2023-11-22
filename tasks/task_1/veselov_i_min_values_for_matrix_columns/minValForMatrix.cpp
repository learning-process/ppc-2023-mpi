// Copyright 2023 Veselov Ilya
#include <mpi.h>
#include <random>
#include "task_1/veselov_i_min_values_for_matrix_columns/minValForMatrix.h"

std::vector<std::vector<int>> randomMatrix(size_t n, size_t m, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            matrix[i][j] = dis(gen);
    }
    return matrix;
}

void MinInCol(const std::vector<std::vector<int>>& matrix, size_t n, size_t m, std::vector<int> res) {
  for (int j = 0; j < m; ++j) {
        int min_ = matrix[0][j];
        for (int i = 1; i < n; ++i) {
            if (matrix[i][j] < min_)
                min_ = matrix[i][j];
        }
        res[j] = min_;
    }
}

void MinInColPar(const std::vector<std::vector<int>>& matrix, size_t n, size_t m, std::vector<int> res) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> flattenedMatrix;
    for (const auto& row : matrix)
        flattenedMatrix.insert(flattenedMatrix.end(), row.begin(), row.end());
    MPI_Bcast(flattenedMatrix.data(), n * m, MPI_INT, 0, MPI_COMM_WORLD);
    for (int j = 0; j < m; ++j) {
        int localMin = 999999;
        for (int i = rank; i < n; i += size)
            if (matrix[i][j] < localMin)
                localMin = matrix[i][j];
        MPI_Reduce(&localMin, &res[j], 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    }
}
