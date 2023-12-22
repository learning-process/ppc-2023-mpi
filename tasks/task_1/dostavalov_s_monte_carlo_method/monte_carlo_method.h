// Copyright 2023 Dostavalov Semen
#ifndef TASKS_TASK_1_DOSTAVALOV_S_MONTE_CARLO_METHOD_MONTE_CARLO_METHOD_H_
#define TASKS_TASK_1_DOSTAVALOV_S_MONTE_CARLO_METHOD_MONTE_CARLO_METHOD_H_

#include <mpi.h>
#include <math.h>
#include <random>
#include <functional>

double monteCarlo(int low, int high, int count, double (*f)(double));

#endif  // TASKS_TASK_1_DOSTAVALOV_S_MONTE_CARLO_METHOD_MONTE_CARLO_METHOD_H_
