// Copyright 2023 Korablev Nikita
#ifndef TASKS_TASK_1_KORABLEV_INT_MONT_INTEGRAL_H_
#define TASKS_TASK_1_KORABLEV_INT_MONT_INTEGRAL_H_

typedef double (*func)(double);

double fsin(double x);
double fcos(double x);
double f_two_sin_cos(double x);

double integral(double a, double b, int N, func f);
double parallel_integral(double a, double b, int N, func f);

#endif  // TASKS_TASK_1_KORABLEV_INT_MONT_INTEGRAL_H
