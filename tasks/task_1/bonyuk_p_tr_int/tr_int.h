// Copyright 2023 Bonyuk Petr
#ifndef TASKS_TASK_1_BONYUK_P_TR_INT_TR_INT_H_
#define TASKS_TASK_1_BONYUK_P_TR_INT_TR_INT_H_

typedef double(*functional)(double);
double const_function(double x);
double standard_function(double x);
double complex_function(double x);
double complex_sqrt_function(double x);
double complex_sin_cos_function(double x);
double get_area(double a, double b, functional f, int n);
double TrapecIntegr(double a, double b, functional f, int N);

#endif  // TASKS_TASK_1_BONYUK_P_TR_INT_TR_INT_H_
