// Copyright 2023 Filatov Maxim

#ifndef TASKS_TASK_1_FILATOV_M_MAX_VALUE_MATRIX_BY_COLUMNS_MAX_VALUE_MATRIX_BY_COLUMNS_H_
#define TASKS_TASK_1_FILATOV_M_MAX_VALUE_MATRIX_BY_COLUMNS_MAX_VALUE_MATRIX_BY_COLUMNS_H_

#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <limits>

#define DEFAULT_RANDOM_MAX 10000
#define DEFAULT_RANDOM_MIN -10000

std::vector<int> generateRandomMatrix(
    std::size_t n,
    std::size_t m,
    int min = DEFAULT_RANDOM_MIN,
    int max = DEFAULT_RANDOM_MAX);

std::vector<int> findMaxInColumns(
    const std::vector<int>& matrix,
    std::size_t rows,
    std::size_t columns
);

std::vector<int> calculateParallelMaxInColumns(
    const std::vector<int> &matrix,
    std::size_t numRows,
    std::size_t numCols
);

#endif  // TASKS_TASK_1_FILATOV_M_MAX_VALUE_MATRIX_BY_COLUMNS_MAX_VALUE_MATRIX_BY_COLUMNS_H_
