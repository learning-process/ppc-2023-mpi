// Copyright 2023 Nogin Denis

#ifndef TASKS_TASK_1_NOGIN_D_MOST_DIFFERENT_ELEMENTS_ALGO_H_
#define TASKS_TASK_1_NOGIN_D_MOST_DIFFERENT_ELEMENTS_ALGO_H_

#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <utility>

void maxDifFunc(void *in, void *inout, int *len, MPI_Datatype *datatype);

std::pair<int, int> sequentialMostDifferentNeighborElements(const std::vector<int> &vec);

std::pair<int, int> parallelMostDifferentNeighborElements(const std::vector<int> &vec);

std::vector<int> randomVector(int sizeVec, int minValue, int maxValue);

#endif  // TASKS_TASK_1_NOGIN_D_MOST_DIFFERENT_ELEMENTS_ALGO_H_
