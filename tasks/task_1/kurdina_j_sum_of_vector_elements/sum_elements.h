// Copyright 2023 Kurdina Julia
#ifndef TASKS_TASK_1_KURDINA_J_SUM_OF_VECTOR_ELEMENTS_SUM_ELEMENTS_H_
#define TASKS_TASK_1_KURDINA_J_SUM_OF_VECTOR_ELEMENTS_SUM_ELEMENTS_H_

#include <mpi.h>
#include <vector>

std::vector<int> get_rand_matrix(int columns, int rows, int l, int r);
int get_sum_parallel(const std::vector<int>& M, int rows);
int get_sum(const std::vector<int>& M);

#endif  // TASKS_TASK_1_KURDINA_J_SUM_OF_VECTOR_ELEMENTS_SUM_ELEMENTS_H_
