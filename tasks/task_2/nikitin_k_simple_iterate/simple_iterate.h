// Copyright 2023 Nikitin Kirill

#ifndef TASKS_TASK_2_NIKITIN_K_SIMPLE_ITERATE_SIMPLE_ITERATE_H_
#define TASKS_TASK_2_NIKITIN_K_SIMPLE_ITERATE_SIMPLE_ITERATE_H_

#include <mpi.h>
#include <cmath>
#include <vector>

std::vector<double> SimpleIterSeq(std::vector<double> a, std::vector<double> b,
                                  double epsilon);
std::vector<double> SimpleIterPar(std::vector<double> a, std::vector<double> b,
                                  double epsilon, MPI_Comm comm);

#endif  // TASKS_TASK_2_NIKITIN_K_SIMPLE_ITERATE_SIMPLE_ITERATE_H_
