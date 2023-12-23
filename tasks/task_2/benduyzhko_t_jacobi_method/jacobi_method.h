// Copyright 2023 Benduyzhko Tatiana
#ifndef TASKS_TASK_2_BENDUYZHKO_T_JACOBI_METHOD_JACOBI_METHOD_H_
#define TASKS_TASK_2_BENDUYZHKO_T_JACOBI_METHOD_JACOBI_METHOD_H_

#include <mpi.h>
#include <vector>

using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;

const double EPSILON = 0.00001;

Vector calculateJacobiSequentially(
    const Matrix& A,
    const Vector& b
);

Vector calculateJacobiParallel(
    const Matrix& A,
    const Vector& b
);

#endif  // TASKS_TASK_2_BENDUYZHKO_T_JACOBI_METHOD_JACOBI_METHOD_H_
