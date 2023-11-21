// Copyright 2023 Safronov Mikhail
#include <mpi.h>
#include <iostream>
#include <tuple>
#include <vector>
#include <limits>
#include "task_1/safronov_nearest_neighbor_elements/nearest_neighbor_elements.h"


std::tuple<int, int, int> findClosestNeighbors(
    const std::vector<int>& values, int rank) {
    if (values.empty()) {
        return std::make_tuple(0, -1, -1);
    }

    int min_diff = std::numeric_limits<int>::max();
    int min_index = -1;

    for (int i = 0; i < values.size() - 1; ++i) {
        int diff = std::abs(values[i + 1] - values[i]);
        if (diff < min_diff) {
            min_diff = diff;
            min_index = i;
        }
    }


    int closest_diff, prev_index, next_index;
    MPI_Allreduce(&min_diff, &closest_diff, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&min_index, &prev_index, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&min_index, &next_index, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);


    if (rank == 0) {
        return std::make_tuple(closest_diff, prev_index, next_index);
    } else {
        return std::make_tuple(0, -1, -1);
    }
}
