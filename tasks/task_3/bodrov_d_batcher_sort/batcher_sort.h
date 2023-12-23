// Copyright 2023 Bodrov Daniil
#ifndef TASKS_TASK_3_BODROV_D_BATCHER_SORT_BATCHER_SORT_H_
#define TASKS_TASK_3_BODROV_D_BATCHER_SORT_BATCHER_SORT_H_

#include <mpi.h>
#include <algorithm>
#include <vector>

void batcherMerge(std::vector<int> *arr, int l, int m, int r);
void batcherSort(std::vector<int> *arr, int l, int r);
void parallelBatcherSort(std::vector<int> *arr, int l, int r);

#endif  // TASKS_TASK_3_BODROV_D_BATCHER_SORT_BATCHER_SORT_H_
