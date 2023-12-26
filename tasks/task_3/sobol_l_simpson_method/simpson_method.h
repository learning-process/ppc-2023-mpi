// Copyright 2023 Sobol Lubov

#ifndef TASKS_TASK_3_SOBOL_L_SIMPSON_METHOD_SIMPSON_METHOD_H_
#define TASKS_TASK_3_SOBOL_L_SIMPSON_METHOD_SIMPSON_METHOD_H_

#include <mpi.h>
#include <cmath>
#include <iostream>

double parallelSimpsonMethod(double start, double end, double lower,
                             double upper, int intervals);
double simpsonMethod(double start, double end, double lower,
                     double upper, int intervals);

#endif  // TASKS_TASK_3_SOBOL_L_SIMPSON_METHOD_SIMPSON_METHOD_H_
