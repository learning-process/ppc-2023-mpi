// Copyright 2023 Makhinya Danil
#ifndef TASKS_TASK_3_MAKHINYA_D_CONJUGATE_GRADIENT_METHOD_GRADIENT_METHOD_H_
#define TASKS_TASK_3_MAKHINYA_D_CONJUGATE_GRADIENT_METHOD_GRADIENT_METHOD_H_

#include <vector>

#define MAX_COUNT_ITER 100000
#define EPS 1e-4

using matrix = std::vector<std::vector<double>>;
using vector = std::vector<double>;

vector solve_parallel(const matrix& A, const vector& b);
double measure(const vector& r);
vector operator* (double c, const vector& b);
vector operator+ (const vector& a, const vector& b);
vector operator- (const vector& a, const vector& b);

#endif  // TASKS_TASK_3_MAKHINYA_D_CONJUGATE_GRADIENT_METHOD_GRADIENT_METHOD_H_
