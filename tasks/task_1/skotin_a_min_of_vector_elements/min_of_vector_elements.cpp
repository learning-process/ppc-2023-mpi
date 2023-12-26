// Copyright 2023 Skotin Alexander
#include "task_1/skotin_a_min_of_vector_elements/min_of_vector_elements.h"
#include <mpi.h>
#include <random>
#include <climits>

int findMinElementMPI(const std::vector<int>& vec, int vecSize) {
    int worldRank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int localMin = vec.empty() ? INT_MAX : vec[0];
    for (int i = 1; i < vec.size(); ++i) {
        if (vec[i] < localMin) {
            localMin = vec[i];
        }
    }

    int globalMin;
    MPI_Reduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    return globalMin;
}

std::vector<int> generateRandomVector(int size, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minElem, maxElem);

    std::vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = distrib(gen);
    }

    return vec;
}
