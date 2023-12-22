// Copyright 2023 Moiseev Nikita
#include <mpi.h>
#include <stdlib.h>
#include <random>
#include <ctime>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include "task_3/moiseev_n_moors/moors.h"

static int getRandomOffset() {
    std::random_device rd;
    std::mt19937 gen(rd());
    const int low = -999;
    const int up = 999;
    std::uniform_int_distribution<int> dist(low, up);
    return dist(gen);
}

const int INFINITY_VALUE = 1000000000;

std::vector<int> generateRandomWeightedGraph(int size) {
    std::mt19937 generator;
    std::uniform_int_distribution<int> weightDistribution(1, 10000);

    generator.seed(static_cast<unsigned>(time(0)) + getRandomOffset());

    std::vector<int> graph(size * size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            graph[j + i * size] = weightDistribution(generator);
            if (graph[j + i * size] == 0) graph[j + i * size] = INFINITY_VALUE;
            if (i == j) graph[j + i * size] = 0;
        }
    return graph;
}

std::vector<int> transposeGraph(const std::vector<int>& graph, int size) {
    std::vector<int> transposedGraph(size * size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            transposedGraph[i * size + j] = graph[j * size + i];
    return transposedGraph;
}

std::vector<int> parallelMooreAlgorithm(const std::vector<int>& graph, int source, int* hasNegativeCycle) {
    const int MINUS_INFINITY = -1000000000;

    int commSize, commRank;
    int size = static_cast<int>(std::sqrt(static_cast<int>(graph.size())));
    std::vector<int> distances(size, INFINITY_VALUE);
    distances[source] = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
    const int delta = size / commSize;
    const int remainder = size % commSize;
    int localSize = delta + (commRank < remainder ? 1 : 0);
    std::vector<int> localGraph(localSize * size);
    std::vector<int> localDistances(localSize, INFINITY_VALUE);

    if (localGraph.empty())
        localGraph.resize(1);

    std::vector<int> sendCounts(commSize);
    std::vector<int> displacements(commSize);
    std::vector<int> sendCountsDistances(commSize);
    std::vector<int> displacementsDistances(commSize);

    displacements[0] = displacementsDistances[0] = 0;
    for (int i = 0; i < commSize; ++i) {
        sendCounts[i] = (delta + (i < remainder ? 1 : 0)) * size;
        sendCountsDistances[i] = delta + (i < remainder ? 1 : 0);
        if (i > 0) {
            displacements[i] = (displacements[i - 1] + sendCounts[i - 1]);
            displacementsDistances[i] = displacementsDistances[i - 1] + sendCountsDistances[i - 1];
        }
    }

    int root = -1;
    for (int i = 0; i < commSize - 1; ++i)
        if (source >= displacements[i] / size)
            root++;
    if (commRank == root)
        localDistances[source - displacements[commRank] / size] = 0;

    std::vector<int> sendGraph;
    if (commRank == 0)
        sendGraph = transposeGraph(graph, size);

    MPI_Scatterv(sendGraph.data(), sendCounts.data(), displacements.data(),
        MPI_INT, &localGraph[0], localSize * size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < size - 1; ++i) {
        for (int k = 0; k < sendCountsDistances[commRank]; ++k) {
            for (int j = 0; j < size; ++j)
                if ((distances[j] < INFINITY_VALUE) && (localGraph[k * size + j] < INFINITY_VALUE))
                    if (localDistances[k] > distances[j] + localGraph[k * size + j])
                        localDistances[k] = std::max(distances[j] + localGraph[k * size + j], MINUS_INFINITY);
        }
        MPI_Allgatherv(localDistances.data(), localSize, MPI_INT, distances.data(),
            sendCountsDistances.data(), displacementsDistances.data(), MPI_INT, MPI_COMM_WORLD);
    }

    if (hasNegativeCycle) {
        int hasNegative = 0;
        *hasNegativeCycle = 0;
        for (int k = 0; k < sendCountsDistances[commRank]; ++k)
            for (int j = 0; j < size; ++j)
                if ((distances[j] < INFINITY_VALUE) && (localGraph[k * size + j] < INFINITY_VALUE)) {
                    if (localDistances[k] > distances[j] + localGraph[k * size + j]) {
                        localDistances[k] = MINUS_INFINITY;
                        hasNegative = 1;
                    }
                }
        MPI_Reduce(&hasNegative, hasNegativeCycle, 1, MPI_INT, MPI_LOR, 0, MPI_COMM_WORLD);
        MPI_Allgatherv(localDistances.data(), localSize, MPI_INT, distances.data(),
            sendCountsDistances.data(), displacementsDistances.data(), MPI_INT, MPI_COMM_WORLD);
    }
    return distances;
}

std::vector<int> sequentialMooreAlgorithm(const std::vector<int>& graph, int source, int* hasNegativeCycle) {
    const int MINUS_INFINITY = -1000000000;

    int size = static_cast<int>(std::sqrt(static_cast<int>(graph.size())));
    if (source < 0 || source >= size)
        throw - 1;
    std::vector<int> distances(size, INFINITY_VALUE);
    distances[source] = 0;

    for (int i = 0; i < size - 1; ++i)
        for (int j = 0; j < size; ++j)
            for (int k = 0; k < size; ++k)
                if ((distances[j] < INFINITY_VALUE) && (graph[k + j * size] < INFINITY_VALUE))
                    if (distances[k] > distances[j] + graph[k + j * size])
                        distances[k] = std::max(distances[j] + graph[k + j * size], MINUS_INFINITY);

    if (hasNegativeCycle) {
        *hasNegativeCycle = 0;
        for (int j = 0; j < size; ++j)
            for (int k = 0; k < size; ++k)
                if ((distances[j] < INFINITY_VALUE) && (graph[k + j * size] < INFINITY_VALUE)) {
                    if (distances[k] > distances[j] + graph[k + j * size]) {
                        distances[k] = MINUS_INFINITY;
                        *hasNegativeCycle = 1;
                    }
                }
    }
    return distances;
}
