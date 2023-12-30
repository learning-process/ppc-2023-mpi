// Copyright 2023 Gontsov Roman
#ifndef TASKS_TASK_1_GONTSOV_R_INTEGRATE_TRAPEZ_INTEGRATE_TRAPEZ_H_
#define TASKS_TASK_1_GONTSOV_R_INTEGRATE_TRAPEZ_INTEGRATE_TRAPEZ_H_

#include <functional>

double getParallelIntegration(const std::function<double(double)>& f,
    double a, double b, int n);
double getSequentialIntegration(const std::function<double(double)>& f,
    double a, double b, int n);

#endif  // TASKS_TASK_1_GONTSOV_R_INTEGRATE_TRAPEZ_INTEGRATE_TRAPEZ_H_
