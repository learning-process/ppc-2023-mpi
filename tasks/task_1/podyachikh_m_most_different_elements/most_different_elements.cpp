// Copyright 2023 Podyachikh Mikhail
#include <mpi.h>
#include <random>
#include <algorithm>
#include <numeric>
#include "task_1/podyachikh_m_most_different_elements/most_different_elements.h"

std::vector<int> VectorRandomNumbers(int vector_size, int min_value, int max_value) {
    std::vector<int> vec(vector_size);
    for (auto &element : vec) {
        element = min_value + (std::rand() % (max_value - min_value + 1));
    }
    return vec;
}

int SequentialGetMostDifferentElements(const std::vector<int> &vec) {
    int max_diff = 0;
    for (int i = 1; i < vec.size(); i++) {
        max_diff = std::max(max_diff, abs(vec[i] - vec[i - 1]));
    }
    return max_diff;
}

int ParallelGetMostDifferentElements(const std::vector<int> &vec) {
    int size_world, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int part_size = vec.size() / size_world;
    std::vector<int> send_counts(size_world, part_size);
    std::fill(send_counts.begin(), send_counts.begin() + vec.size() % size_world, part_size + 1);

    std::vector<int> displacement(size_world);
    std::partial_sum(send_counts.begin(), send_counts.end() - 1, displacement.begin() + 1);

    for (int i = 0; i < size_world - 1; i++) {
        if (send_counts[i + 1] == 0) break;
        send_counts[i]++;
    }

    std::vector<int> rec_vector(send_counts[rank]);

    MPI_Scatterv(vec.data(), send_counts.data(), displacement.data(), MPI_INT,
                 rec_vector.data(), send_counts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    int local_max = SequentialGetMostDifferentElements(rec_vector);

    int max_diff;
    MPI_Reduce(&local_max, &max_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    return max_diff;
}
