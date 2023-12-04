// Copyright 2023 Ulyanov Daniil
#ifndef TASKS_TASK_1_ULYANOV_D_MATRIX_SUM_MATRIX_SUM_H_
#define TASKS_TASK_1_ULYANOV_D_MATRIX_SUM_MATRIX_SUM_H_

#include <mpi.h>
#include <vector>

int sequentialSum(const std::vector<int> &v);
int parallelSum(const std::vector<int> &v);

#endif  // TASKS_TASK_1_ULYANOV_D_MATRIX_SUM_MATRIX_SUM_H_
