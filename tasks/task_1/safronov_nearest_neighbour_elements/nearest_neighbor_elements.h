// Copyright 2023 Safronov Mikhail
#ifndef TASKS_TASK_1_SAFRONOV_NEAREST_NEIGHBOR_ELEMENTS_NEAREST_NEIGHBOR_ELEMENTS_H_
#define TASKS_TASK_1_SAFRONOV_NEAREST_NEIGHBOR_ELEMENTS_NEAREST_NEIGHBOR_ELEMENTS_H_

#include <mpi.h>

#include <vector>
#include <tuple>

std::tuple<int, int, int> findClosestNeighbors(
    const std::vector<int>& values, int rank);

#endif  // TASKS_TASK_1_SAFRONOV_NEAREST_NEIGHBOR_ELEMENTS_NEAREST_NEIGHBOR_ELEMENTS_H_
