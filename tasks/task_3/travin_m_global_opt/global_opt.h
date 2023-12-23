// Copyright 2023 Travin Maxim
#ifndef TASKS_TASK_3_TRAVIN_M_GLOBAL_OPT_GLOBAL_OPT_H_
#define TASKS_TASK_3_TRAVIN_M_GLOBAL_OPT_GLOBAL_OPT_H_

#include <mpi.h>
#include <iostream>
#include <cmath>
#include <functional>
#include <utility>
#include <vector>
#include <algorithm>
#include <limits>

namespace TAG {
    const int FINDED = 1;
    const int UNFINDED = 2;
}

const double parameterR = 2.0;

using RAndIndex = std::pair<double, int>;

double getMiddle(double a, double b);
double getParameterM(const std::vector<double>& points,
    std::function<double(double)> fun);
double getParameterL(double parameterM, double parameterR);
std::vector<RAndIndex> getParametersR(
    const std::vector<double>& points,
    std::function<double(double)> fun,
    double parameter_m
);
double getNextX(double point0, double point1,
    std::function<double(double)> fun,
    double parameter_m);
double getNextX(
    const std::vector<double>& points,
    std::function<double(double)> fun,
    int indexOfMaxR,
    double parameter_m
);

double globalSearchParallel(std::function<double(double)> fun,
    double a,
    double b,
    double epsilone);

#endif  // TASKS_TASK_3_TRAVIN_M_GLOBAL_OPT_GLOBAL_OPT_H_
