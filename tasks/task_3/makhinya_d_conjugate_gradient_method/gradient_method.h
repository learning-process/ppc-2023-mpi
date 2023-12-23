// Copyright 2023 Makhinya Danil
#ifndef TASKS_TASK_2_MAKHINYA_D_GRADIENT_METHOD_GRADIENT_METHOD_H_
#define TASKS_TASK_2_MAKHINYA_D_GRADIENT_METHOD_GRADIENT_METHOD_H_

#include <vector>

using matrix = std::vector<std::vector<double>>;
using vector = std::vector<double>;

vector solve_parallel(const matrix& A, const vector& b);

#endif  // TASKS_TASK_2_MAKHINYA_D_GRADIENT_METHOD_GRADIENT_METHOD_H_
