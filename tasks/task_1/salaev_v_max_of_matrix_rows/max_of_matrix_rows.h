// Copyright 2023 Salaev Vladislav
#ifndef TASKS_TASK_1_SALAEV_V_MAX_OF_MATRIX_ROWS_MAX_OF_MATRIX_ROWS_H_
#define TASKS_TASK_1_SALAEV_V_MAX_OF_MATRIX_ROWS_MAX_OF_MATRIX_ROWS_H_

#include <vector>
#include <boost/serialization/vector.hpp>

std::vector<int> getRandomMatrix(int m, int n);
std::vector<size_t> getParallelMaxInRows(
std::vector<int> glob_matr, int m, int n);
std::vector<size_t> getSequentialMaxInRows(std::vector<int> matr, int m);

#endif  // TASKS_TASK_1_SALAEV_V_MAX_OF_MATRIX_ROWS_MAX_OF_MATRIX_ROWS_H_
