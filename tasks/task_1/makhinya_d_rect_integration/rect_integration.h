// Copyright 2023 Makhinya Danil
#ifndef TASKS_TASK_1_MAKHINYA_D_RECT_INTERATION_H_
#define TASKS_TASK_1_MAKHINYA_D_RECT_INTERATION_H_

#include <functional>

double getSequentialIntegral(std::function<double(double)> func, 
                             double a, double b, uint32_t count_partitions);
double getParallelIntegral(std::function<double(double)> func, 
                           double a, double b, uint32_t count_partitions);

#endif  // TASKS_TASK_1_MAKHINYA_D_RECT_INTERATION_H_
