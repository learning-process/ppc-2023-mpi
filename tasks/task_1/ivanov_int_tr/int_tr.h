// Copyright 2023 Ivanov Nikita
#ifndef TASKS_TASK_1_IVANOV_INT_TR_INT_TR_H_
#define TASKS_TASK_1_IVANOV_INT_TR_INT_TR_H_

typedef double (*func)(double);

double sin_f(double x);
double sin2_f(double x);
double hardfn_f(double x);
double hardfn2_f(double x);
double sin_cos_f(double x);

double trapezium(double a, double b, func f);

double getParallelOperations(double a, double b, int N, func f);

double get_area(double a, func f, int steps_count, double step);

#endif  // TASKS_TASK_1_IVANOV_INT_TR_INT_TR_H_
