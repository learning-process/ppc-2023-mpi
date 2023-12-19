// Copyright 2023 Shmelev Ivan
#ifndef TASKS_TASK_2_SHMELEV_I_SIMPLE_ITER_METHOD_SIMPLE_ITER_METHOD_H_
#define TASKS_TASK_2_SHMELEV_I_SIMPLE_ITER_METHOD_SIMPLE_ITER_METHOD_H_

#include <mpi.h>

#include <cmath>
#include <vector>

std::vector<double> SimpleIterSeq(std::vector<double> a, std::vector<double> b,
                                  double epsilon);
std::vector<double> SimpleIterPar(std::vector<double> a, std::vector<double> b,
                                  double epsilon, MPI_Comm comm);

#endif  // TASKS_TASK_2_SHMELEV_I_SIMPLE_ITER_METHOD_SIMPLE_ITER_METHOD_H_
