// Copyright 2023 Korablev Nikita
#ifndef TASKS_TASK_2_KORABLEV_ZEIDEL_ZEIDEL_H_
#define TASKS_TASK_2_KORABLEV_ZEIDEL_ZEIDEL_H_

#include <iostream>
#include <random>
#include <iostream>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

double* zeidel(double* matrix, int n, double epsilon);
double* parallel_zeidel(double* matrix, int n, double epsilon);

#endif  // TASKS_TASK_2_KORABLEV_ZEIDEL_ZEIDEL_H_
