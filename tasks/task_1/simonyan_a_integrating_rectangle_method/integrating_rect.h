// Copyright 2023 Simonyan Suren
#ifndef TASKS_TASK_1_SIMONYAN_A_INTEGRATING_RECTANGLE_METHOD_INTEGRATING_RECT_H_
#define TASKS_TASK_1_SIMONYAN_A_INTEGRATING_RECTANGLE_METHOD_INTEGRATING_RECT_H_

#include <mpi.h>
#include <iostream>
#include <vector>

typedef double (*func)(double);

double sin_f(double x);
double cos_f(double x);
double hardfn_f(double x);
double hardfn2_f(double x);
double sin_cos(double x);

double RectangleIntegrate(double leftbound,
    double rightbound, double step, func f);

#endif  // TASKS_TASK_1_SIMONYAN_A_INTEGRATING_RECTANGLE_METHOD_INTEGRATING_RECT_H_
