// Copyright 2023 Kozlov Mikhail
#ifndef TASKS_TASK_2_KOZLOV_M_SEIDEL_METHOD_SEIDEL_METHOD_H_
#define TASKS_TASK_2_KOZLOV_M_SEIDEL_METHOD_SEIDEL_METHOD_H_

#include <mpi.h>
#include <vector>

std::vector<double> seidel_parallel(std::vector<std::vector<double>> A, std::vector<double> B, int n);
std::vector<double> seidel_seq(std::vector<std::vector<double>> A, const std::vector<double> B, int n);

#endif  // TASKS_TASK_2_KOZLOV_M_SEIDEL_METHOD_SEIDEL_METHOD_H_
