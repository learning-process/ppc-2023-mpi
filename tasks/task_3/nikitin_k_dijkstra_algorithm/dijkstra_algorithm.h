// Copyright 2023 Nikitin Kirill

#ifndef TASKS_TASK_3_NIKITIN_K_DIJKSTRA_ALGORITHM_DIJKSTRA_ALGORITHM_H_
#define TASKS_TASK_3_NIKITIN_K_DIJKSTRA_ALGORITHM_DIJKSTRA_ALGORITHM_H_
#include <mpi.h>
#include <vector>
#include <queue>
#include <random>
#include <climits>
#include <utility>


std::vector<int> DijkstraSeq(std::vector<int> graph,
    int source, int size);
std::vector<int> DijkstraParallel(std::vector<int> graph,
    int source, int size);

#endif  // TASKS_TASK_3_NIKITIN_K_DIJKSTRA_ALGORITHM_DIJKSTRA_ALGORITHM_H_
