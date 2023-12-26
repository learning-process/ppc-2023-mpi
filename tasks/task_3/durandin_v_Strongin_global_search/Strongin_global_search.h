// Copyright 2023 Durandin Vladimir

#ifndef TASKS_TASK_3_DURANDIN_V_STRONGIN_GLOBAL_SEARCH_STRONGIN_GLOBAL_SEARCH_H_
#define TASKS_TASK_3_DURANDIN_V_STRONGIN_GLOBAL_SEARCH_STRONGIN_GLOBAL_SEARCH_H_

#include <mpi.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <random>
#include <vector>

namespace global_search {
double seq_Strongin_alghorithm(double x0, double x1, double eps);
double paral_Strongin_algorithm(double x0, double x1, double eps);
}  // namespace global_search

#endif  // TASKS_TASK_3_DURANDIN_V_STRONGIN_GLOBAL_SEARCH_STRONGIN_GLOBAL_SEARCH_H_
