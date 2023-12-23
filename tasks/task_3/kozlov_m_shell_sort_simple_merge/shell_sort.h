// Copyright 2023 Kozlov Mikhail
#ifndef TASKS_TASK_3_KOZLOV_M_SHELL_SORT_SIMPLE_MERGE_SHELL_SORT_H_
#define TASKS_TASK_3_KOZLOV_M_SHELL_SORT_SIMPLE_MERGE_SHELL_SORT_H_

#include <mpi.h>
#include <vector>

void merge(std::vector<int> arr, int low, int mid, int high);
void seq_sort(std::vector<int> arr);
void parallel_sort(std::vector<int> arr);

#endif  // TASKS_TASK_3_KOZLOV_M_SHELL_SORT_SIMPLE_MERGE_SHELL_SORT_H_
