// Copyright 2023 Filatov Maxim

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/filatov_m_max_value_matrix_by_columns/max_value_matrix_by_columns.h"

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
    std::vector<int> resultVector(columns);
    std::size_t elementCountForProcess = (rows / communicator.size()) * columns;
    std::size_t remainingElementsForRootProcess = (
        rows % communicator.size()) * columns;
    std::vector<int> localSizesForProcess(
        communicator.size(),
        elementCountForProcess);
    std::vector<int> localMatrixPartForProcess(elementCountForProcess);
    localSizesForProcess[0] += remainingElementsForRootProcess;

    if (!elementCountForProcess) {
        localMatrixPartForProcess = matrix;
    } else {
        if (!communicator.rank()) {
            localMatrixPartForProcess.resize(localSizesForProcess[0]);
            boost::mpi::scatterv(
                communicator,
                matrix,
                localSizesForProcess,
                localMatrixPartForProcess.data(),
                0);
        } else {
            boost::mpi::scatterv(
                communicator,
                localMatrixPartForProcess.data(),
                localSizesForProcess[communicator.rank()],
                0);
        }
    }

    std::vector<int> localColumnMax(columns, std::numeric_limits<int>::min());
    for (
        int i = 0;
        i < localSizesForProcess[communicator.rank()] / columns;
        i++) {
        int rowStart = i * columns;
        int rowEnd = rowStart + columns;
        std::vector<int> row(
            matrix.begin() + rowStart,
            matrix.begin() + rowEnd);
        std::transform(row.begin(),
            row.end(),
            localColumnMax.begin(),
            localColumnMax.begin(),
            [](int a, int b) {
            return std::max(a, b);
        });
    }

    if (!elementCountForProcess) {
        resultVector = (!communicator.rank() ? localColumnMax : resultVector);
    } else {
        boost::mpi::reduce(
            communicator,
            &localColumnMax.front(),
            columns,
            &resultVector.front(),
            boost::mpi::maximum<int>(),
            0);
    }

    return resultVector;
}
