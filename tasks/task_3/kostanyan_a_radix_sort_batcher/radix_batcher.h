// Copyright 2023 Kostanyan Arsen

#ifndef TASKS_TASK_3_KOSTANYAN_A_RADIX_SORT_BATCHER_RADIX_BATCHER_H_
#define TASKS_TASK_3_KOSTANYAN_A_RADIX_SORT_BATCHER_RADIX_BATCHER_H_

#include <mpi.h>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <cstring>

#include "task_3/kostanyan_a_radix_sort_batcher/batcher_merge.h"

std::vector<double> generateRandomVector(int size);
std::vector<double> parallel_radix_batcher_sort(std::vector<double> to_sort_vec);

void radix_sort(double * arr, int size);

#endif  // TASKS_TASK_3_KOSTANYAN_A_RADIX_SORT_BATCHER_RADIX_BATCHER_H_
