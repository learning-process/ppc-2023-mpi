// Copyright 2023 Bonyuk Petr
#ifndef TASKS_TASK_3_BONYUK_P_RADIX_BACHER_RADIX_BACHER_H_
#define TASKS_TASK_3_BONYUK_P_RADIX_BACHER_RADIX_BACHER_H_

#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>
void radixSort(const std::vector<int>& nums);
void radixSortUnsigned(const std::vector<int>& nums);
int checkMPIResult(int result);
void compareExchange(const std::vector<int>& local_nums, int i, int j, int dir);
void batcherMerge(const std::vector<int>& local_nums, int numProcs, int myRank);

#endif  // TASKS_TASK_3_BONYUK_P_RADIX_BACHER_RADIX_BACHER_H_
