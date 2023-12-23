// Copyright 2023 Petrov Maksim
#ifndef TASKS_TASK_2_PETROV_M_SIMPLE_ITERATION_METHOD_SIMPLE_ITERATION_METHOD_H_
#define TASKS_TASK_2_PETROV_M_SIMPLE_ITERATION_METHOD_SIMPLE_ITERATION_METHOD_H_

#include <vector>

std::vector<double> get_rand_vec(int n);

std::vector<double> get_rand_matrix(int n);

std::vector<double> get_parallel_simple_iteration_method(const std::vector<double>& A_vector,
    const std::vector<double>& b_vector, int n, int max_numb_iter, double epsilon);

std::vector<double> get_sequential_simple_iteration_method(const std::vector<double>& A_authentic,
    const std::vector<double>& b_authentic, int n, int max_numb_iter, double epsilon);

std::vector<double> mult_for_verification_aprox(const std::vector<double>& A_vector,
    const std::vector<double>& b_vector, int n);

#endif  // TASKS_TASK_2_PETROV_M_SIMPLE_ITERATION_METHOD_SIMPLE_ITERATION_METHOD_H_
