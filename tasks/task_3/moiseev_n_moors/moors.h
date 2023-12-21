// Copyright 2023 Moiseev Nikita
#ifndef TASKS_TASK_3_MOISEEV_N_MOORS_MOORS_H_
#define TASKS_TASK_3_MOISEEV_N_MOORS_MOORS_H_

#include <mpi.h>

static int getRandomOffset();
std::vector<int> generateRandomWeightedGraph(int size);
std::vector<int> transposeGraph(const std::vector<int>& g, int n);
std::vector<int> sequentialMooreAlgorithm(const std::vector<int>& g, int source, int* flag = nullptr);
std::vector<int> parallelMooreAlgorithm(const std::vector<int>& g, int source, int* flag = nullptr);

#endif  // TASKS_TASK_3_MOISEEV_N_MOORS_MOORS_H_
