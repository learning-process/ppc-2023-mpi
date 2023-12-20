// Copyright 2023 Skotin Alexander
#include "task_1/skotin_a_min_of_vector_elements/min_of_vector_elements.h"
#include <mpi.h>
#include <random>
#include <limits>
#include <algorithm>

int VectorMinFinder::parallelMin(const std::vector<int>& data) {
    int localMin = std::numeric_limits<int>::max();
    if (!data.empty()) {
        localMin = *std::min_element(data.begin(), data.end());
    }

    int globalMin;
    MPI_Allreduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    return globalMin;
}

int VectorMinFinder::sequentialMin(const std::vector<int>& data) {
    if (data.empty()) {
        return std::numeric_limits<int>::max();
    }
    return *std::min_element(data.begin(), data.end());
}

std::vector<int> VectorMinFinder::generateRandomVector(size_t size, int min, int max) {
    std::vector<int> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    for (auto& it : vec) {
        it = distrib(gen);
    }

    return vec;
}
