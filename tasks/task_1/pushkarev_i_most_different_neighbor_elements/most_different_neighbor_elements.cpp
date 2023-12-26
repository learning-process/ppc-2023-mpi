// Copyright 2023 Pushkarev Ilya

#include "task_1/pushkarev_i_most_different_neighbor_elements/most_different_neighbor_elements.h"
#include <random>
#include <algorithm>

int maxDifferenceSeq(const std::vector<int> &vec) {
    int maxDiff = 0;
    for (int i = 0; i < vec.size() - 1; i++) {
        int diff = std::abs(vec[i] - vec[i + 1]);
        if (diff > maxDiff) {
            maxDiff = diff;
        }
    }

    return maxDiff;
}

int maxDifferencePar(const std::vector<int> &vec) {
    int numProc;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    int vecSize = vec.size();

    std::vector<int> recv_counts(numProc);
    std::vector<int> displs(numProc);

    int chunk_size = vecSize / numProc;
    int remainder = vecSize % numProc;

    for (int i = 0; i < numProc; i++) {
        recv_counts[i] = chunk_size;
        if (i < remainder) {
            recv_counts[i]++;
        }
        displs[i] = (i > 0) ? (displs[i - 1] + recv_counts[i - 1]) : 0;
    }

    std::vector<int> recv_data(recv_counts[rankProc]);
    MPI_Scatterv(vec.data(), recv_counts.data(), displs.data(), MPI_INT, recv_data.data(),
                recv_counts[rankProc], MPI_INT, 0, MPI_COMM_WORLD);

    int localMaxDiff = maxDifferenceSeq(recv_data);
    int globalMaxDiff = 0;

    MPI_Reduce(&localMaxDiff, &globalMaxDiff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rankProc == 0) {
        int maxDiffOriginal = maxDifferenceSeq(vec);
        globalMaxDiff = std::max(globalMaxDiff, maxDiffOriginal);
    }

    return globalMaxDiff;
}

std::vector<int> create_random_vector(size_t size, int lower_bound, int upper_bound) {
    std::vector<int> random_vector(size);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribution(lower_bound, upper_bound);

    for (int &element : random_vector) {
        element = distribution(gen);
    }

    return random_vector;
}
