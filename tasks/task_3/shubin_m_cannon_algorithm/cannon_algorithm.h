// Copyright 2023 Shubin Mikhail
#ifndef TASKS_TASK_3_SHUBIN_M_CANNON_ALGORITHM_CANNON_ALGORITHM_H_
#define TASKS_TASK_3_SHUBIN_M_CANNON_ALGORITHM_CANNON_ALGORITHM_H_

#include <mpi.h>
#include <cmath>
#include <vector>

double round_double(double val, int zeros);

void mat_mult_cons(int n, const std::vector<double>& mat_in_1,
    const std::vector<double>& mat_in_2, std::vector<double>* mat_out);
void mat_mult_cannon(int n, const std::vector<double>& mat_in_1,
    const std::vector<double>& mat_in_2, std::vector<double>* mat_out);

#endif  // TASKS_TASK_3_SHUBIN_M_CANNON_ALGORITHM_CANNON_ALGORITHM_H_