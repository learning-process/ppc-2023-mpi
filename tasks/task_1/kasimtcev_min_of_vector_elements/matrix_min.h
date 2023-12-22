// Copyright 2023 Kasimtcev Roman
#ifndef TASKS_TASK_1_KASIMTCEV_MIN_OF_VECTOR_ELEMENTS_MATRIX_MIN_H_
#define TASKS_TASK_1_KASIMTCEV_MIN_OF_VECTOR_ELEMENTS_MATRIX_MIN_H_

#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <climits>

std::vector<int> CreateRandomMatrix(int rows, int columns);
int FndMin(std::vector<int> global_matrix);
int FndMinParallel(std::vector<int> global_matrix);

#endif  // TASKS_TASK_1_KASIMTCEV_MIN_OF_VECTOR_ELEMENTS_MATRIX_MIN_H_
