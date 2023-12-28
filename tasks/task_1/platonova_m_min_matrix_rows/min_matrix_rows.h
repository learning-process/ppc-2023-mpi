// Copyright 2023 Platonova Maria
#ifndef TASKS_TASK_1_PLATONOVA_M_MIN_MATRIX_ROWS_MIN_MATRIX_ROWS_H_
#define TASKS_TASK_1_PLATONOVA_M_MIN_MATRIX_ROWS_MIN_MATRIX_ROWS_H_

#include <vector>
#include <boost/serialization/vector.hpp>

std::vector<int> getRandomMatrix(int m, int n);
std::vector<size_t> getParallelMin(std::vector<int> glob_matr, int m, int n);
std::vector<size_t> getSequentialMin(std::vector<int> matr, int m);

#endif  // TASKS_TASK_1_PLATONOVA_M_MIN_MATRIX_ROWS_MIN_MATRIX_ROWS_H_
