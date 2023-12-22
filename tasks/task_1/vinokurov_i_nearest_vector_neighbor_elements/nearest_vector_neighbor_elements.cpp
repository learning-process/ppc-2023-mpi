// Copyright 2023 Vinokurov Ivan
#include <mpi.h>
#include <iostream>
#include <vector>
#include <utility>
#include "task_1/vinokurov_i_nearest_vector_neighbor_elements/nearest_vector_neighbor_elements.h"

#define INT_MAX 2147483647

std::pair<int, int> funcFindClosestNeighborsMPI(const std::vector<int>& _data) {
    if (_data.size() == 0) {
        return { -1, -1 };
    }
    int minDifference = INT_MAX;
    int minIndex = -1;
    int closestNeighborIndex;

    for (int i = 0; i < _data.size(); i++) {
        int difference = std::abs(_data[i] - _data[i + 1]);
        if (difference < minDifference) {
            minIndex = i;
            minDifference = difference;
        }
    }

    MPI_Allreduce(&minIndex, &closestNeighborIndex, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    return { closestNeighborIndex, closestNeighborIndex + 1 };
}
