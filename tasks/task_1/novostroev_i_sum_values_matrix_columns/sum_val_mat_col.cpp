// Copyright 2023 Novostroev Ivan
#include "task_1/novostroev_i_sum_values_matrix_columns/sum_val_mat_col.h"
#include <mpi.h>
#include <vector>
#include <iostream>
#include <utility>

std::pair<std::vector<int>, std::vector<int>> distribution
(bool flag, const size_t row, const size_t col) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> counts(size, 0);
    std::vector<int> offsets(size, 0);
    for (size_t i = 0; i < size; i++) {
        auto dist = countAndOffsetDistribution(row, i);
        auto localCount = dist.first;
        auto localOffset = dist.second;

        if (flag == true) {
            counts[i] = localCount * col;
            offsets[i] = localOffset * col;
        } else {
            counts[i] = localCount;
            offsets[i] = localOffset;
        }
    }
    return std::make_pair(counts, offsets);
}

std::vector<double> transposition
(const std::vector<double>& matrix, const size_t row, const size_t col) {
    std::vector<double> result(col * row, 0.0);
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            result[j * row + i] = matrix[i * col + j];
        }
    }
    return result;
}

std::vector<double> sumMatrixColEquential
(const std::vector<double>& matrix, const size_t row, const size_t col) {
    std::vector<double> colSum(row, 0.0);
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            colSum[i] += matrix[i * col + j];
        }
    }
    return colSum;
}

std::vector<double> sumMatrixColParallel
(const std::vector<double>& matrix, const size_t row, const size_t col) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> localMatrix(
        countAndOffsetDistribution(row, rank).first * col);

    std::pair<std::vector<int>, std::vector<int>> dist1;
    std::vector<int> counts1, offsets1;
    if (rank == 0) {
        dist1 = distribution(true, row, col);
        counts1 = dist1.first;
        offsets1 = dist1.second;
    }
    MPI_Scatterv(matrix.data(), counts1.data(), offsets1.data(),
     MPI_DOUBLE, localMatrix.data(), localMatrix.size(),
      MPI_DOUBLE, 0, MPI_COMM_WORLD);

    auto localResult = sumMatrixColEquential(localMatrix,
     localMatrix.size() / col, col);

    std::vector<double> globalResult(row, 0.0);

    std::pair<std::vector<int>, std::vector<int>> dist2;
    std::vector<int> counts2, offsets2;
    if (rank == 0) {
        dist2 = distribution(false, row, col);
        counts2 = dist2.first;
        offsets2 = dist2.second;
    }
    MPI_Gatherv(localResult.data(), localResult.size(),
     MPI_DOUBLE, globalResult.data(), counts2.data(), offsets2.data(),
      MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return globalResult;
}

std::pair<int, int> countAndOffsetDistribution
(const size_t row, int rank) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rowPerProc = row / size;
    int remainder = row % size;

    auto localCount = rowPerProc + (rank < remainder ? 1 : 0);
    auto localOffset = rank * rowPerProc +
     (rank < remainder ? rank : remainder);

    return std::make_pair(localCount, localOffset);
}
