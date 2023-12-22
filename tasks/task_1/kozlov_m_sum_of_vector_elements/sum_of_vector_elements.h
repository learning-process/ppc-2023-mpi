// Copyright 2023 Kozlov Mikhail
#ifndef TASKS_TASK_1_KOZLOV_M_SUM_OF_VECTOR_ELEMENTS_SUM_OF_VECTOR_ELEMENTS_H_
#define TASKS_TASK_1_KOZLOV_M_SUM_OF_VECTOR_ELEMENTS_SUM_OF_VECTOR_ELEMENTS_H_

#include <mpi.h>
#include <vector>

int sum_of_vec_elem_seq(const std::vector<int> &vec);
int sum_of_vec_elem_par(const std::vector<int> &vec);
std::vector<int> get_random_vector(int sz);

#endif  // TASKS_TASK_1_KOZLOV_M_SUM_OF_VECTOR_ELEMENTS_SUM_OF_VECTOR_ELEMENTS_H_
