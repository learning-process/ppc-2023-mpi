// Copyright 2023 Afanasyev Aleksey
#ifndef TASKS_TASK_1_AFANASYEV_A_MAX_VALUES_BY_MATRIX_ROWS_H_
#define TASKS_TASK_1_AFANASYEV_A_MAX_VALUES_BY_MATRIX_ROWS_H_

#include <vector>
std::vector<int> CalculateMaxValuesByMatrixRowsSequential(const std::vector<int>& matrix, size_t rowSize, size_t rowCount);
std::vector<int> CalculateMaxValuesByMatrixRowsParallel(const std::vector<int>& matrix, size_t rowSize, size_t rowCount);

#endif  // TASKS_TASK_1_AFANASYEV_A_MAX_VALUES_BY_MATRIX_ROWS_H_
