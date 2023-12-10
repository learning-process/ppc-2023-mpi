// Copyright 2023 Smirnov Leonid
#include "task_1/smirnov_l_matrix_col_sum/matrix_col_sum.h"
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

std::vector<double> MatrixColsSumSequential
(const std::vector<double>& matrix, const size_t rows, const size_t cols)
{
    std::vector<double> colsSum(rows, 0.0);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            colsSum[i] += matrix[i * cols + j];
        }
    }
    return colsSum;
}

//
std::pair<int, int> countAndOffsetDistribution
(const size_t rows, int rank) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int rowsPerProc = rows / size;
    int remainder = rows % size;
    
    auto localCount = rowsPerProc + (rank < remainder ? 1 : 0);
    auto localOffset = rank * rowsPerProc + (rank < remainder ? rank : remainder);


    return std::make_pair(localCount, localOffset);
}

//
std::pair<std::vector<int>, std::vector<int>> distribution(bool flag, const size_t rows, const size_t cols){
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
        }
        else {
            counts[i] = localCount;
            offsets[i] = localOffset;
        }
    }

    return std::make_pair(counts, offsets);
}

//
std::vector<double> MatrixColsSumParallel
(const std::vector<double>& matrix, const size_t rows, const size_t cols)
{
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> localMatrix(countAndOffsetDistribution(rows, rank).first * cols);

    /*if (rank == 0) {*/
        auto dist = distribution(true, rows, cols);
        auto counts = dist.first;
        auto offsets = dist.second;
        MPI_Scatterv(
            matrix.data(),
            counts.data(),
            offsets.data(),
            MPI_DOUBLE,
            localMatrix.data(),
            localMatrix.size(),
            MPI_DOUBLE,
            0,
            MPI_COMM_WORLD
        );
    /*} */
    /*else {
        MPI_Scatterv(
            nullptr,
            nullptr,
            nullptr,
            MPI_DOUBLE,
            localMatrix.data(),
            localMatrix.size(),
            MPI_DOUBLE,
            0,
            MPI_COMM_WORLD
        );
    }*/

    auto localResult = MatrixColsSumSequential(localMatrix, localMatrix.size() / cols, cols);

    /*if (rank != 0) {
        MPI_Gatherv(
            localResult.data(),
            localResult.size(),
            MPI_DOUBLE,
            nullptr,
            nullptr,
            nullptr,
            MPI_DOUBLE,
            0,
            MPI_COMM_WORLD
            );
    }
    else {*/
    std::vector<double> globalResult(rows, 0.0);
    auto dist = distribution(false, rows, cols);
    auto counts = dist.first;
    auto offsets = dist.second;
    MPI_Gatherv(
        localResult.data(),
        localResult.size(),
        MPI_DOUBLE,
        globalResult.data(),
        counts.data(),
        offsets.data(),
        MPI_DOUBLE,
        0,
        MPI_COMM_WORLD
    );
    return globalResult;
}

