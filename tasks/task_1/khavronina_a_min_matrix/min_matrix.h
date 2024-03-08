// Copyright 2023 Khavronina Alexandra

#ifndef TASKS_TASK_1_KHAVRONINA_A_MIN_MATRIX_MIN_MATRIX_H_
#define TASKS_TASK_1_KHAVRONINA_A_MIN_MATRIX_MIN_MATRIX_H_

#include <vector>
#include <boost/serialization/vector.hpp>

std::vector<int> gen_random_matrix(int m, int n);
std::vector<size_t> find_min_values_in_matrix_rows_parallel(std::vector<int> global_matrix, int m, int n);
std::vector<size_t> get_min_values_in_matrix_rows(std::vector<int> matrix, int m);

#endif  // TASKS_TASK_1_KHAVRONINA_A_MIN_MATRIX_MIN_MATRIX_H_
