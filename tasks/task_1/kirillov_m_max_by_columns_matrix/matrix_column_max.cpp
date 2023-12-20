// Copyright 2023 Kirillov Maxim
#include <vector>
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

std::pair<int, int> getColIndexAndCount(size_t columns);
std::vector<int> getRandomMatrix(int rows, int columns){
    std::random_device dev;
    std::mt19937 gen(dev());
    int size = rows * columns;
    std::vector<int> matrix(size);
    for (int i = 0; i < size; i++) {
        matrix[i] = gen() % 100;
    }
    return matrix;
}

std::vector<int> getSequentialMaxInColumns(std::vector<int>&matrix, size_t rows, size_t columns){
    std::vector<int> maxValues(columns, std::numeric_limits<int>::min());
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++ ) {
            maxValues[j] =  std::max(maxValues[j], matrix[i * columns + j]);
        }
    }
    return maxValues;
}

std::vector<int> getParallelMaxInColumns(std::vector<int>&matrix, size_t rows, size_t columns){
    boost::mpi::communicator world;
    if (world.rank() != 0){
        matrix.resize(rows*columns);
    }
    boost::mpi::broadcast(world, matrix.data(), rows * columns, 0);
    auto [colsCount, colIndex] = getColIndexAndCount(columns);
    std::vector<int> localMaxValues(colsCount);
    for (int i = colIndex; i < colsCount + colIndex; i++){
        for (int j = 0; j < rows; j++) {
            localMaxValues[i - colIndex] = std::max(localMaxValues[i - colIndex], matrix[j * columns + i]);
        }
    }
    if (world.rank() != 0){
        boost::mpi::scatter(world, localMaxValues.data(), colsCount, 0);
    }
    else {
        std::vector<int> globalMaxValues(columns);
        boost::mpi::scatter(world, globalMaxValues, localMaxValues.data(), colsCount, 0);
        return globalMaxValues;
    }
    return {};
}

std::pair<int, int> getColIndexAndCount(size_t columns){
    boost::mpi::communicator world;
    int size = world.size();
    int rank = world.rank();
    int colsPerProcess = columns / size;
    int reminder = columns % size;
    // 6 / 3 = 2 (colsperpr)
    // 8 / 3 = 2, 8 % 3 = 2,
    int colsCount = colsPerProcess + (rank < reminder ? 1 : 0);
    int colIndex = colsPerProcess * rank + (rank < reminder ? rank : reminder);
    return std::make_pair(colsCount, colIndex);
}
