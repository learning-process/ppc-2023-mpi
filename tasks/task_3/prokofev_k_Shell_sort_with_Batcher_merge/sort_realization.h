// Copyright 2023 Prokofev Kirill
#ifndef TASKS_TASK_3_PROKOFEV_K_SHELL_SORT_WITH_BATCHER_MERGE_SORT_REALIZATION_H_
#define TASKS_TASK_3_PROKOFEV_K_SHELL_SORT_WITH_BATCHER_MERGE_SORT_REALIZATION_H_

#include <mpi.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <random>

void ShellSortSeq(std::vector<int>* vec);
void ShellSortParallel(std::vector<int>* vec);
void BatcherMerge(std::vector<int>* vec, int l, int m, int r);
void BatcherOddEvenMerge(std::vector<int>* vec, int start, int end, int dist);
void BatcherSort(std::vector<int>* arr);
std::vector<int> GenerateRandomVector(int n);

#endif  // TASKS_TASK_3_PROKOFEV_K_SHELL_SORT_WITH_BATCHER_MERGE_SORT_REALIZATION_H_
