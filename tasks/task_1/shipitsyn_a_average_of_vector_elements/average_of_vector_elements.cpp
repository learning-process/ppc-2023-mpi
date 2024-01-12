// Copyright 2023 Shipitsyn Alexey
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/shipitsyn_a_average_of_vector_elements/average_of_vector_elements.h"

std::vector<int> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

double getSequentialAverage(std::vector<int> array) {
    double sum = std::accumulate(array.begin(), array.end(), 0.0);
    return sum / array.size();;
}

double getParallelAverage(std::vector<int> array) {
    boost::mpi::communicator world;
    int localSize = array.size() / world.size();
    std::vector<int> sizes(world.size(), localSize);

    if (array.size() % world.size()) {
        int elementsLeft = array.size() - world.size() * localSize;
        for (int i = 0; i < elementsLeft; ++i) {
            sizes[i]++;
        }
    }

    std::vector<int> localArray(sizes[world.rank()], 0);
    boost::mpi::scatterv(world, array.data(), sizes, localArray.data(), 0);
    double localSum = std::accumulate(localArray.begin(), localArray.end(), 0.0);
    double globalSum = 0;
    boost::mpi::reduce(world, localSum, globalSum, std::plus<double>(), 0);
    return globalSum / array.size();
}
