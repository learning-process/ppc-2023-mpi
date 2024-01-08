// Copyright 2023 Pushkarev Ilya

#ifndef TASKS_TASK_2_PUSHKAREV_I_JACOBI_METHOD_JACOBI_METHOD_H_
#define TASKS_TASK_2_PUSHKAREV_I_JACOBI_METHOD_JACOBI_METHOD_H_
#include <mpi.h>
#include <vector>


std::vector<double> Jacobi(const std::vector<double>& A,
    const std::vector<double>& b,
    int num_iterations);
std::vector<double> create_random_matrix(size_t size, double lower_bound, double upper_bound);
std::vector<double> create_random_vector(size_t size, double lower_bound, double upper_bound);

#endif  // TASKS_TASK_2_PUSHKAREV_I_JACOBI_METHOD_JACOBI_METHOD_H_

