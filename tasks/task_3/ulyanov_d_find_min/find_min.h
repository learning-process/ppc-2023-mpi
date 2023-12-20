// Copyright 2023 Ulyanov Daniil
#ifndef TASKS_TASK_3_ULYANOV_D_FIND_MIN_FIND_MIN_H_
#define TASKS_TASK_3_ULYANOV_D_FIND_MIN_FIND_MIN_H_

#include <mpi.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <utility>

enum TAG_STATUS {
    SIMPLE_TAG = 0,
    FOUND_SOLUTION_TAG = 1,
    SEARCH_SOLUTION_TAG = 2
};

double getLipschitzParam(double param_M);
double getMParam(const std::vector<double>& x_points,
  std::function<double(double)> target_func);
std::vector<std::pair<int, double>> getIntervalCharactR(
  const std::vector<double>& x_points,
  std::function<double(double)> target_func,
  double param_m);
double getNextPointX(double pointX0, double pointX1,
    std::function<double(double)> target_func, double param_m);
void sortVecR(std::vector<std::pair<int, double>>* charactR);
double getGlobalSearchSequantial(double a, double b,
    std::function<double(double)> target_func, double E);
double getGlobalSearchParallel(double a, double b,
    std::function<double(double)> target_func, double E);

#endif  // TASKS_TASK_3_ULYANOV_D_FIND_MIN_FIND_MIN_H_