// Copyright 2023 Kurdina Julia
#ifndef TASKS_TASK_2_KURDINA_J_COLUMN_WISE_COLUMN_WISE_H_
#define TASKS_TASK_2_KURDINA_J_COLUMN_WISE_COLUMN_WISE_H_

#include <mpi.h>
#include <vector>

std::vector<int> get_rand_matrix(int columns, int rows, int l, int r);
std::vector<int> transpose_M(const std::vector<int>& M, int columns, int rows);
std::vector<int> columnwise_sum_parallel(const std::vector<int>& M, const std::vector<int>& b, int rows, int columns);
std::vector<int> columnwise_sum(const std::vector<int>& M, const std::vector<int>& b, int rows, int columns);

#endif  // TASKS_TASK_2_KURDINA_J_COLUMN_WISE_COLUMN_WISE_H_
