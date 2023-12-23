// Copyright 2023 Ionova Ekaterina
#ifndef TASKS_TASK_1_IONOVA_E_INT_RECT_INT_RECT_H_
#define TASKS_TASK_1_IONOVA_E_INT_RECT_INT_RECT_H_

#include <math.h>
#include <mpi.h>

#include <functional>

bool isEqual(double valueOne, double valueTwo, double epsilon);
double seqIntegrationRectangle(double a, double b, double n,
                               std::function<double(double)> func);
double parIntegrationRectangle(double a, double b, uint32_t n,
                               std::function<double(double)> func,
                               MPI_Comm comm);

#endif  // TASKS_TASK_1_IONOVA_E_INT_RECT_INT_RECT_H_
