// Copyright 2023 Volodin Evgeniy
#ifndef TASKS_TASK_1_VOLODIN_E_MATRIX_MAX_MATRIX_MAX_H_
#define TASKS_TASK_1_VOLODIN_E_MATRIX_MAX_MATRIX_MAX_H_

#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <climits>

std::vector<int> createRandomMatrix(int rows, int columns);
int findMax(std::vector<int> global_matrix);
int findMaxParallel(std::vector<int> global_matrix);

#endif  // TASKS_TASK_1_VOLODIN_E_MATRIX_MAX_MATRIX_MAX_H_
