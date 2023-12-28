// Copyright 2023 Gontsov Roman
#ifndef TASKS_TASK_3_GONTSOV_R_MONTE_CARLO_INTEGRATE_MONTE_CARLO_INTEGRATE_H_
#define TASKS_TASK_3_GONTSOV_R_MONTE_CARLO_INTEGRATE_MONTE_CARLO_INTEGRATE_H_

#include <vector>
#include <functional>

double getIntegralMonteCarlo(
const std::function<double(const std::vector<double>&)>& f,
 std::vector<double> a, std::vector<double> b, int number_points);

double getIntegralMonteCarloStd(
    const std::function<double(const std::vector<double>&)>& f,
    std::vector<double> a, std::vector<double> b, int number_points);

#endif  // TASKS_TASK_3_GONTSOV_R_MONTE_CARLO_INTEGRATE_MONTE_CARLO_INTEGRATE_H_
