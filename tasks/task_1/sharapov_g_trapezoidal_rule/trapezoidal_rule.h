// Copyright 2023 Sharapov Georgiy
#ifndef TASKS_TASK_1_SHARAPOV_G_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_
#define TASKS_TASK_1_SHARAPOV_G_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_

#include <functional>

double integralTrapezoidalParallel(double a, double b, int64_t l, std::function<double(double)> func);

#endif  // TASKS_TASK_1_SHARAPOV_G_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_
