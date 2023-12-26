// Copyright 2023 Videneva Ekaterina
#ifndef TASKS_TASK_3_VIDENEVA_E_SEARCH_SPACE_SEARCH_SPACE_H_
#define TASKS_TASK_3_VIDENEVA_E_SEARCH_SPACE_SEARCH_SPACE_H_

#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>
#include <utility>

double GetGlobalOptimization(const double i, const double j, std::function<double(double*)> func,
    const int part, const double e);
double GetGlobalOptimizationParallel(const double i, const double j, std::function<double(double*)> func,
    const int part, const double e);

#endif  // TASKS_TASK_3_VIDENEVA_E_SEARCH_SPACE_SEARCH_SPACE_H_
