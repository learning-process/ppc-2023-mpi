// Copyright 2023 Ivanchenko Aleksei
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/ivanchenko_a_max_across_columns/max_across_columns.h"

std::vector<int> getRandomMatrix(size_t rows, size_t columns, int minElem, int maxElem) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> distrib(minElem, maxElem);

    size_t size = rows*columns;
    std::vector<int> vec(size);
    for (int  i = 0; i < size; i++) {
        vec[i] = distrib(gen);
    }
    return vec;
}
std::vector<int> getMaxSequentional(const std::vector<int>& matrix, size_t rows, size_t columns) {
    std::vector<int> result(columns, INT_MIN);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            result[j] = std::max(result[j], matrix[i*columns + j]);
        }
    }
    return result;
}
std::vector<int> getMaxParallel(const std::vector<int>& matrix, size_t rows, size_t columns) {
    boost::mpi::communicator comm;
    std::vector<int> res(columns);
    size_t t1 = (rows / comm.size()) * columns;
    size_t t2 = (rows % comm.size()) * columns;
    std::vector<int> localSizes(comm.size(), t1);
    std::vector<int> localMatrix(t1);
    localSizes[0] += t2;
    if (t1 == 0) {
        localMatrix = matrix;
    } else {
        if (comm.rank() == 0) {
            localMatrix.resize(localSizes[0]);
            boost::mpi::scatterv(comm, matrix, localSizes, localMatrix.data(), 0);
        } else {
            boost::mpi::scatterv(comm, localMatrix.data(), localSizes[comm.rank()], 0);
        }
    }
    std::vector<int> localMax(columns, INT_MIN);
    for (int i = 0; i < localSizes[comm.rank()] / columns; i++) {
        for (int j = 0; j < columns; j++) {
            localMax[j] = std::max(localMax[j], localMatrix[i*columns + j]);
        }
    }
    if (t1 == 0) {
        if (comm.rank() == 0) {
            res = localMax;
        }
    } else {
        boost::mpi::reduce(comm, &localMax.front(), columns, &res.front(), boost::mpi::maximum<int>(), 0);
    }
    return res;
}
