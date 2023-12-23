// Copyright 2023 Khramov Ivan
#ifndef TASKS_TASK_3_KHRAMOV_I_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
#define TASKS_TASK_3_KHRAMOV_I_GLOBAL_SEARCH_GLOBAL_SEARCH_H_

#define MAX_VALUE std::numeric_limits<double>::max()
#define MIN_VALUE std::numeric_limits<double>::min()
#define MAX_ITERATIONS 100000
#define r_param 2.0
#define PI 3.141592

enum TAG_STATUS {
    SIMPLE_TAG = 0,
    FOUND_SOLUTION_TAG = 1,
    SEARCH_SOLUTION_TAG = 2
};

#include <mpi.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <utility>

double getLipschitzParam(
    double param_M,
    double param_r);
double getMParam(
    const std::vector<double>& x_points,
    std::function<double(double)> target_func);
std::vector<std::pair<int, double>> getIntervalCharactR(
    const std::vector<double>& x_points,
    std::function<double(double)> target_func,
    double param_m);
double getNextPointX(
    double pointX0,
    double pointX1,
    std::function<double(double)> target_func,
    double param_m);
void sortVecR(
    std::vector<std::pair<int, double>>* charactR);
double getGlobalSearchSequantial(
    double a,
    double b,
    std::function<double(double)> target_func,
    double eps);
double getGlobalSearchParallel(
    double a,
    double b,
    std::function<double(double)> target_func,
    double eps);

#endif  // TASKS_TASK_3_KHRAMOV_I_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
