// Copyright 2023 Shishkina Valeria
#ifndef TASKS_TASK_3_SHISHKINA_L_METODSIMPSONA_METODSIMPSONA_H_
#define TASKS_TASK_3_SHISHKINA_L_METODSIMPSONA_METODSIMPSONA_H_

#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <iostream>

double f(double x, double y);
double parallelSimpson(double a, double b, double c, double d, int n);
double simpson(double a, double b, double c, double d, int n);

#endif  // TASKS_TASK_3_SHISHKINA_L_METODSIMPSONA_METODSIMPSONA_H_
