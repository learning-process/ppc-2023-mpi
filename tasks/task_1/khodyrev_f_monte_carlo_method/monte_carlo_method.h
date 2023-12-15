// Copyright 2023 Khodyrev Fedor

#ifndef TASKS_TASK_1_KHODYREV_F_MONTE_CARLO_METHOD_MONTE_CARLO_METHOD_H_
#define TASKS_TASK_1_KHODYREV_F_MONTE_CARLO_METHOD_MONTE_CARLO_METHOD_H_

#include <mpi.h>
#include <random>
#include <functional>

double seq_monte_carlo_integration(int num_points, double lower_limit,
double upper_limit, std::function<double(double)> func);
double par_monte_carlo_integration(int num_points, double lower_limit,
double upper_limit, std::function<double(double)> func);
#endif  // TASKS_TASK_1_KHODYREV_F_MONTE_CARLO_METHOD_MONTE_CARLO_METHOD_H_
