// Copyright 2023 Troitskiy Alexandr
#include <task_1/troitskiy_a_matrix_col_summa/matrix_col_summa.h>
#include <mpi.h>
#include <vector>
#include <iostream>
#include <utility>

std::vector<double> transposition
(const std::vector<double>& matrix, const size_t rows, const size_t cols) {
    std::vector<double> result(cols * rows, 0.0);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            result[j * rows + i] = matrix[i * cols + j];
        }
    }
    return result;
}

std::vector<double> matrixColsSumSequential
(const std::vector<double>& matrix, const size_t rows, const size_t cols) {
    std::vector<double> colsSum(rows, 0.0);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            colsSum[i] += matrix[i * cols + j];
        }
    }
    return colsSum;
}

std::pair<int, int> countAndOffsetDistribution
(const size_t rows, int rank) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rowsPerProc = rows / size;
    int remainder = rows % size;

    auto localCount = rowsPerProc + (rank < remainder ? 1 : 0);
    auto localOffset = rank * rowsPerProc +
        (rank < remainder ? rank : remainder);

    return std::make_pair(localCount, localOffset);
}

std::pair<std::vector<int>, std::vector<int>> distribution
(bool flag, const size_t rows, const size_t cols) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> counts(size, 0);
    std::vector<int> offsets(size, 0);
    for (size_t i = 0; i < size; i++) {
        auto dist = countAndOffsetDistribution(rows, i);
        auto localCount = dist.first;
        auto localOffset = dist.second;

        if (flag == true) {
            counts[i] = localCount * cols;
            offsets[i] = localOffset * cols;
        } else {
            counts[i] = localCount;
            offsets[i] = localOffset;
        }
    }
    return std::make_pair(counts, offsets);
}

//
std::vector<double> matrixColsSumParallel
(const std::vector<double>& matrix, const size_t rows, const size_t cols) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> localMatrix(
        countAndOffsetDistribution(rows, rank).first * cols);

    std::pair<std::vector<int>, std::vector<int>> dist1;
    std::vector<int> counts1, offsets1;
    if (rank == 0) {
        dist1 = distribution(true, rows, cols);
        counts1 = dist1.first;
        offsets1 = dist1.second;
    }
    MPI_Scatterv(matrix.data(), counts1.data(), offsets1.data(), MPI_DOUBLE,
                 localMatrix.data(), localMatrix.size(), MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    auto localResult = matrixColsSumSequential(
        localMatrix, localMatrix.size() / cols, cols);

    std::vector<double> globalResult(rows, 0.0);

    std::pair<std::vector<int>, std::vector<int>> dist2;
    std::vector<int> counts2, offsets2;
    if (rank == 0) {
        dist2 = distribution(false, rows, cols);
        counts2 = dist2.first;
        offsets2 = dist2.second;
    }

    MPI_Gatherv(localResult.data(), localResult.size(), MPI_DOUBLE,
        globalResult.data(), counts2.data(), offsets2.data(), MPI_DOUBLE,
        0, MPI_COMM_WORLD);

    return globalResult;
}
