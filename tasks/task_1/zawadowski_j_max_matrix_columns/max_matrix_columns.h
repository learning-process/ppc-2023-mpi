// Copyright 2023 Zawadowski Jan
#ifndef TASKS_TASK_1_ZAWADOWSKI_J_MAX_MATRIX_COLUMNS_MAX_MATRIX_COLUMNS_H_
#define TASKS_TASK_1_ZAWADOWSKI_J_MAX_MATRIX_COLUMNS_MAX_MATRIX_COLUMNS_H_

#include <mpi.h>
#include <algorithm>
#include <vector>
#include <random>
#include <limits>

std::vector<int> fillMatrixRandom(int rows, int cols, int min, int max);
std::vector<int> findMaxByMatrixColumns(std::vector<int> matrix,
    int rows,
    int cols);
std::vector<int> findMaxByMatrixColumnsConsistently(std::vector<int> matrix,
    int rows,
    int cols);

#endif  // TASKS_TASK_1_ZAWADOWSKI_J_MAX_MATRIX_COLUMNS_MAX_MATRIX_COLUMNS_H_
