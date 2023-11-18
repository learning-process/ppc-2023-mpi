// Copyright 2023 Kuznetsov Artem
#ifndef TASKS_TASK_1_KUZNETSOV_A_MOST_DIFFERENT_VALUES_MOST_DIFFERENT_H_
#define TASKS_TASK_1_KUZNETSOV_A_MOST_DIFFERENT_VALUES_MOST_DIFFERENT_H_

#include <mpi.h>

#include <random>
#include <vector>

std::vector<int> create_random_array(size_t size, int low, int up);
int seq_find_most_different(const std::vector<int>& arr);
int par_find_most_different(const std::vector<int>& arr);

#endif  // TASKS_TASK_1_KUZNETSOV_A_MOST_DIFFERENT_VALUES_MOST_DIFFERENT_H_
