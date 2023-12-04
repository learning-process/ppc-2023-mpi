// Copyright 2023 Prokofev Kirill
#ifndef TASKS_TASK_3_PROKOFEV_K_SHELL_SORT_WITH_BATCHER_MERGE_H_
#define TASKS_TASK_3_PROKOFEV_K_SHELL_SORT_WITH_BATCHER_MERGE_H_

#include <mpi.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <random>

void ShellSortSeq(std::vector<int>& vec);
std::vector<int> BatcherMerge(const std::vector<int>& vec_1,
  const std::vector<int>& vec_2);
void ShellSortParallel(std::vector<int>& vec);
std::vector<int> GenerateRandomVector(int n);

#endif  // TASKS_TASK_3_PROKOFEV_K_SHELL_SORT_WITH_BATCHER_MERGE_H_
