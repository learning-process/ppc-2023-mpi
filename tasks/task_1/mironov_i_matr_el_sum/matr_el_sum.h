// Copyright 2023 Mironov Ilya
#ifndef TASKS_TASK_1_MIRONOV_I_MATR_EL_SUM_MATR_EL_SUM_H_
#define TASKS_TASK_1_MIRONOV_I_MATR_EL_SUM_MATR_EL_SUM_H_
#include <vector>

std::vector<int> get_random_matrix(int sz, int a, int b);
int get_sequential_matr_el_sum(const std::vector<int> &matr);
int get_parallel_matr_el_sum(const std::vector<int> &matr, int sz);

#endif  // TASKS_TASK_1_MIRONOV_I_MATR_EL_SUM_MATR_EL_SUM_H_
