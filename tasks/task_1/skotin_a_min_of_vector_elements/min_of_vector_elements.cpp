// Copyright 2023 Skotin Alexander
#include "min_of_vector_elements.h"
#include <mpi.h>
#include <limits>
#include <algorithm>

VectorMinFinder::VectorMinFinder() : worldSize(0), worldRank(0) {}

VectorMinFinder::~VectorMinFinder() {}

void VectorMinFinder::initialize(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
}

void VectorMinFinder::finalize() {
    MPI_Finalize();
}

int VectorMinFinder::findMinParallel(const std::vector<int>& vector) {
    int chunkSize = vector.size() / worldSize;
    std::vector<int> localChunk(chunkSize);

    MPI_Scatter(vector.data(), chunkSize, MPI_INT, localChunk.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    int localMin = std::numeric_limits<int>::max();
    for (int num : localChunk) {
        localMin = std::min(localMin, num);
    }

    int globalMin;
    MPI_Reduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    return (worldRank == 0) ? globalMin : std::numeric_limits<int>::max();
}

int VectorMinFinder::findMinSequential(const std::vector<int>& vector) {
    return *std::min_element(vector.begin(), vector.end());
}
