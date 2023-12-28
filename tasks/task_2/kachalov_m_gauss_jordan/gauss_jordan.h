// Copyright 2023 Kachalov Mikhail
#ifndef TASKS_TASK_2_KACHALOV_M_GAUSS_JORDAN_GAUSS_JORDAN_H_
#define TASKS_TASK_2_KACHALOV_M_GAUSS_JORDAN_GAUSS_JORDAN_H_

#include <ctime>
#include <vector>
#include <random>

std::vector<std::vector<double>> generate_random_matrix(const int& rows, const int& cols);
std::vector<double> solve_linear_system_sequential(
    const std::vector<std::vector<double>>& A, const std::vector<double>& b);
std::vector<double> solve_linear_system_parallel
(const std::vector<std::vector<double>>& A, const std::vector<double>& b);

#endif  // TASKS_TASK_2_KACHALOV_M_GAUSS_JORDAN_GAUSS_JORDAN_H_
