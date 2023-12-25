// Copyright 2023 Filatov Maxim

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/filatov_m_max_value_by_matrix_columns/max_value_matrix_by_columns.h"

struct RandomNumber {
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;
    RandomNumber() {
        gen = std::mt19937(rd());
        dist = std::uniform_int_distribution<int>(
            DEFAULT_RANDOM_MIN, DEFAULT_RANDOM_MAX);
    }
    RandomNumber(int min, int max) {
        gen = std::mt19937(rd());
        dist = std::uniform_int_distribution<int>(min, max);
    }
    int operator()() {
        return dist(gen);
    }
};

std::vector<int> generateRandomMatrix(
    std::size_t row,
    std::size_t col,
    int min,
    int max
) {
    RandomNumber randomNumber;
    std::vector<int> matrix(row * col);
    std::generate(
        matrix.begin(),
        matrix.end(),
        [&](){ return randomNumber(); });
    return matrix;
}

std::vector<int> findMaxInColumns(
    const std::vector<int>& matrix,
    size_t rows,
    size_t columns
) {
    std::vector<int> maxValues(columns, std::numeric_limits<int>::min());
    for (size_t col = 0; col < columns; ++col) {
        std::vector<int> columnValues(rows);
        for (size_t row = 0; row < rows; ++row) {
            columnValues[row] = matrix[row * columns + col];
        }
        maxValues[col] = *std::max_element(
            columnValues.begin(),
            columnValues.end());
    }
    return maxValues;
}

std::vector<int> calculateParallelMaxInColumns(
    const std::vector<int>& matrix,
    std::size_t rows,
    std::size_t columns
) {
    boost::mpi::communicator communicator;
    std::size_t rowCountForProcess = rows / communicator.size();
    std::size_t remainingRows = rows % communicator.size();
    std::size_t localMatrixSize = (communicator.rank() < remainingRows) ?
                                (rowCountForProcess + 1) * columns
                                : rowCountForProcess * columns;
    std::vector<int> localMatrixPart(localMatrixSize);
    std::vector<int> resultVector(columns, std::numeric_limits<int>::min());
    std::vector<int> sendcounts(communicator.size());
    std::vector<int> displs(communicator.size());
    for (size_t i = 0; i < sendcounts.size(); ++i) {
        sendcounts[i] = (i < remainingRows) ?
            (rowCountForProcess + 1) * columns : rowCountForProcess * columns;
        displs[i] = (i == 0) ? 0 : displs[i - 1] + sendcounts[i - 1];
    }
    boost::mpi::scatterv(
        communicator,
        matrix.data(),
        sendcounts,
        displs,
        localMatrixPart.data(),
        localMatrixSize, 0);

    std::vector<int> localMax(columns, std::numeric_limits<int>::min());
    for (std::size_t i = 0; i < localMatrixSize / columns; i++) {
        for (std::size_t j = 0; j < columns; j++) {
            localMax[j] = std::max(
                localMax[j],
                localMatrixPart[i * columns + j]);
        }
    }

    boost::mpi::all_reduce(
        communicator,
        localMax.data(),
        columns,
        resultVector.data(),
        boost::mpi::maximum<int>());

    return resultVector;
}

