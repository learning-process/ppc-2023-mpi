// Copyright 2023 Mironov Ilya
#ifndef TASKS_TASK_3_MIRONOV_I_TRAPEZOID_METHOD_TRAPEZOID_METHOD_H_
#define TASKS_TASK_3_MIRONOV_I_TRAPEZOID_METHOD_TRAPEZOID_METHOD_H_

double plus(double x, double y);
double minus(double x, double y);
double multiple(double x, double y);
double division(double x, double y);
double quadSum(double x, double y);

double sequentialTrapezoidalRule(double xa, double xb, double ya, double yb, int n, double (*func)(double, double));
double parallelTrapezoidalRule(double xa, double xb, double ya, double yb, int n, double (*func)(double, double));

#endif  // TASKS_TASK_3_MIRONOV_I_TRAPEZOID_METHOD_TRAPEZOID_METHOD_H_
