// Copyright 2023 Kutarin Aleksandr
#ifndef TASKS_TASK_1_KUTARIN_A_INTEGRATION_RECTANGLE_METHOD_INTEGRATING_RECT_H_
#define TASKS_TASK_1_KUTARIN_A_INTEGRATION_RECTANGLE_METHOD_INTEGRATING_RECT_H_
#include <mpi.h>
#include <iostream>
#include <vector>

typedef double (*func)(double);

double sin_func(double x);
double cos_func(double x);
double sin_cos(double x);
double hardfn_func(double x);
double hardfn2_func(double x);

double RectangleIntegrate(double left_border,
    double right_border, double step, func f);

#endif  // TASKS_TASK_1_KUTARIN_A_INTEGRATION_RECTANGLE_METHOD_INTEGRATING_RECT_H_
