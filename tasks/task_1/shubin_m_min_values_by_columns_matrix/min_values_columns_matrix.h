// Copyright 2023 Shubin Mikhail
#ifndef TASKS_TASK_1_SHUBIN_M_MIN_VALUES_BY_COLUMNS_MATRIX_MIN_VALUES_COLUMNS_MATRIX_H_
#define TASKS_TASK_1_SHUBIN_M_MIN_VALUES_BY_COLUMNS_MATRIX_MIN_VALUES_COLUMNS_MATRIX_H_

#include <mpi.h>
#include <vector>
#include <random>
#include <limits>
#include <algorithm>

std::vector<int> getRandomMatrix(size_t row, size_t col, int min_v, int max_v);
std::vector<int> getColumnMin_seq(const std::vector<int> &matr,
                                  size_t row, size_t col);
std::vector<int> getColumnMin_par(const std::vector<int> &matr,
                                  size_t row, size_t col);

#endif  // TASKS_TASK_1_SHUBIN_M_MIN_VALUES_BY_COLUMNS_MATRIX_MIN_VALUES_COLUMNS_MATRIX_H_
