// Copyright 2023 Alexseev Danila

#ifndef TASKS_TASK_3_ALEXSEEV_D_MONTE_CARLO_INTEGRAL_MONTE_CARLO_INTEGRAL_H_
#define TASKS_TASK_3_ALEXSEEV_D_MONTE_CARLO_INTEGRAL_MONTE_CARLO_INTEGRAL_H_

#include <mpi.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>

typedef double (*func)(const std::vector<double>&);

double func1VarPow(const std::vector<double>& x);
double func1VarCos(const std::vector<double>& x);
double func2VarExp(const std::vector<double>& x);
double func2VarSin(const std::vector<double>& x);
double func2VarSum(const std::vector<double>& x);

double parallelCalculateIntegral(func foo, const int dimensionOfSpace,
    const int pointsCount, const std::vector<double>& lowerLimits, const std::vector<double>& upperLimits);

#endif  // TASKS_TASK_3_ALEXSEEV_D_MONTE_CARLO_INTEGRAL_MONTE_CARLO_INTEGRAL_H_
