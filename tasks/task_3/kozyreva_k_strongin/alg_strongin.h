// Copyright 2023 Kozyreva Katya
#ifndef TASKS_TASK_3_KOZYREVA_K_STRONGIN_ALG_STRONGIN_H_
#define TASKS_TASK_3_KOZYREVA_K_STRONGIN_ALG_STRONGIN_H_

#include <mpi.h>
#include <functional>
#include <utility>

typedef std::pair<double, double> coords;
coords SequentalStrongin(const std::function<double(double)>& func, double a, double b);
coords ParallelStrongin(const std::function<double(double)>& func, double a, double b);


#endif  // TASKS_TASK_3_KOZYREVA_K_STRONGIN_ALG_STRONGIN_H_
