// Copyright 2023 Borovkov Sergey
#ifndef TASKS_TASK_1_BOROVKOV_S_INT_REG_ALG_H_
#define TASKS_TASK_1_BOROVKOV_S_INT_REG_ALG_H_

double integrateSequential(double (*f)(double), double a, double b, int n);
double integrateParallel(double (*f)(double), double a, double b, int n);

#endif  // TASKS_TASK_1_BOROVKOV_S_INT_REG_ALG_H_
