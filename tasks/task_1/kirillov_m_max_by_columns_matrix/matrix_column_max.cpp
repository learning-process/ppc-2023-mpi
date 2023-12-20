// Copyright 2023 Kirillov Maxim
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <utility>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/kirillov_m_max_by_columns_matrix/matrix_column_max.h"

std::pair<int, int> getColIndexAndCount(size_t columns, int rank);
std::vector<int> getRandomMatrix(int rows, int columns) {
    std::random_device dev;
    std::mt19937 gen(dev());
    int size = rows * columns;
    std::vector<int> matrix(size);
    for (int i = 0; i < size; i++) {
        matrix[i] = gen() % 100;
    }
    return matrix;
}

std::vector<int> getSequentialMaxInColumns(const std::vector<int>&matrix,
                                           size_t rows, size_t columns) {
    std::vector<int> maxValues(columns, std::numeric_limits<int>::min());
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            maxValues[j] =  std::max(maxValues[j], matrix[i * columns + j]);
        }
    }
    return maxValues;
}

std::vector<int> getParallelMaxInColumns(const std::vector<int>&matrixc,
                                         size_t rows, size_t columns) {
    boost::mpi::communicator world;
    std::vector<int>matrix(matrixc);
    if (world.rank() != 0) {
        matrix.resize(rows*columns);
    }

    boost::mpi::broadcast(world, matrix.data(), rows * columns, 0);
    auto [colsCount, colIndex] = getColIndexAndCount(columns, world.rank());
    std::vector<int> localMaxValues(colsCount);
    for (int i = colIndex; i < colsCount + colIndex; i++) {
        for (int j = 0; j < rows; j++) {
            localMaxValues[i - colIndex] =
                std::max(localMaxValues[i - colIndex], matrix[j * columns + i]);
        }
    }
    if (world.rank() != 0) {
        boost::mpi::gatherv(world, localMaxValues, 0);
    } else {
        std::vector<int> globalMaxValues(columns);
        std::vector<int> counts(world.size()), offsets(world.size());
        for (int i = 0; i < world.size(); i++) {
            auto [count, offset] = getColIndexAndCount(columns, i);
            counts[i] = count;
            offsets[i] = offset;
        }
        boost::mpi::gatherv(world, localMaxValues,
                            globalMaxValues.data(), counts, offsets, 0);
        return globalMaxValues;
    }
    return {};
}

std::pair<int, int> getColIndexAndCount(size_t columns, int rank) {
    boost::mpi::communicator world;
    int size = world.size();
    int colsPerProcess = columns / size;
    int reminder = columns % size;
    int colsCount = colsPerProcess + (rank < reminder ? 1 : 0);
    int colIndex = colsPerProcess * rank + (rank < reminder ? rank : reminder);
    if (rank >= columns) {
        colIndex = 0;
    }
    return std::make_pair(colsCount, colIndex);
}
