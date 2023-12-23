// Copyright 2023 Kulaev Eugene
#ifndef TASKS_TASK_3_KULAEV_E_GLOBAL_SEARCH_GLOB_SEARCH_H_
#define TASKS_TASK_3_KULAEV_E_GLOBAL_SEARCH_GLOB_SEARCH_H_

#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>
#include <utility>

double GetGlobalOpt(const double a, const double b, std::function<double(double*)> func,
    const int part, const double e);
double GetGlobalOptParallel(const double a, const double b, std::function<double(double*)> func,
    const int part, const double e);

#endif  // TASKS_TASK_3_KULAEV_E_GLOBAL_SEARCH_GLOB_SEARCH_H_
