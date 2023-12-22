// Copyright 2023 Benduyzhko Tatiana
#ifndef TASKS_TASK_3_BENDUYZHKO_T_MOORS_ALGORITM_MOORS_ALGORITM_H_
#define TASKS_TASK_3_BENDUYZHKO_T_MOORS_ALGORITM_MOORS_ALGORITM_H_

#include <mpi.h>
#include <vector>

std::vector<int> getRandomGraph(int size);
std::vector<int> Transpose(const std::vector<int>& g, int n);
std::vector<int> ParallelMoor(const std::vector<int>& g, int source,
                                int* flag = nullptr);
std::vector<int> Moors_algorithm(const std::vector<int>& g, int source,
                                int* flag = nullptr);

#endif  // TASKS_TASK_3_BENDUYZHKO_T_MOORS_ALGORITM_MOORS_ALGORITM_H_
