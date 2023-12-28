// Copyright 2023 NEDELIN DMITRY
#ifndef TASKS_TASK_1_NEDELIN_SUM_BY_COLUMNS_MATRIX_SUM_COLUMNS_H_
#define TASKS_TASK_1_NEDELIN_SUM_BY_COLUMNS_MATRIX_SUM_COLUMNS_H_

#include <mpi.h>
#include <random>
#include <vector>

std::vector<int> CreateMatrix(const int rows, const int cols);
std::vector<int> GetSumCols(const std::vector<int>& matrix,
    const int rows, const int cols);
std::vector<int> GetSumColsParallel(const std::vector<int>& matrix,
    const int rows, const int cols);

#endif  // TASKS_TASK_1_NEDELIN_SUM_BY_COLUMNS_MATRIX_SUM_COLUMNS_H_
