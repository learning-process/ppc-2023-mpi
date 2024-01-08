// Copyright 2023 Shemyakina Alesya
#include <mpi.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "task_1/shemyakina_a_most_different_neighbor_elements/most_different_neighbor_elements.h"

std::vector<int> VecRandNum(int vector_size, int min_value, int max_value) {
    std::vector<int> vec(vector_size);
    for (auto &element : vec) {
        element = min_value + (std::rand() % (max_value - min_value + 1));
    }
    return vec;
}

int SequentialGet(const std::vector<int> &vec) {
    int maxDiff = 0;
    for (int i = 1; i < vec.size(); i++) {
        maxDiff = std::max(maxDiff, abs(vec[i] - vec[i - 1]));
    }
    return maxDiff;
}

int ParallelGet(const std::vector<int> &vec) {
    int size_world, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int PartOfSize = vec.size() / size_world;
    std::vector<int> counts(size_world, PartOfSize);
    std::fill(counts.begin(), counts.begin() + vec.size() % size_world, PartOfSize + 1);

    std::vector<int> shift(size_world);
    std::partial_sum(counts.begin(), counts.end() - 1, shift.begin() + 1);

    for (int i = 0; i < size_world - 1; i++) {
        if (counts[i + 1] == 0) break;
        counts[i]++;
    }

    std::vector<int> rec_vector(counts[rank]);

    MPI_Scatterv(vec.data(), counts.data(), shift.data(), MPI_INT,
                 rec_vector.data(), counts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    int localMax = SequentialGet(rec_vector);

    int maxDiff;
    MPI_Reduce(&localMax, &maxDiff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    return maxDiff;
}
