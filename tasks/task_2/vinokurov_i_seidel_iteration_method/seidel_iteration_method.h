// Copyright 2023 Vinokurov Ivan
#ifndef TASKS_TASK_2_VINOKUROV_I_SEIDEL_ITERATION_METHOD_SEIDEL_ITERATION_METHOD_H_
#define TASKS_TASK_2_VINOKUROV_I_SEIDEL_ITERATION_METHOD_SEIDEL_ITERATION_METHOD_H_

#include <mpi.h>
#include <vector>
#include <cmath>

std::vector<double> funcSystemSolveSeidelMPI(const std::vector<std::vector<double>>& _mtxA,
                                             const std::vector<double>& _vectorB, int _numRows, double _eps);
#endif  //  TASKS_TASK_2_VINOKUROV_I_SEIDEL_ITERATION_METHOD_SEIDEL_ITERATION_METHOD_H_
