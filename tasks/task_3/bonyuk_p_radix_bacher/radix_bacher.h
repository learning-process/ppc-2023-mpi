// Copyright 2023 Bonyuk Petr
#ifndef TASKS_TASK_3_BONYUK_P_RADIX_BACHER_RADIX_BACHER_H_
#define TASKS_TASK_3_BONYUK_P_RADIX_BACHER_RADIX_BACHER_H_

#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>

std::vector<int> radixSort(std::vector<int> nums);
std::vector<int> radixSortUnsigned(std::vector<int> nums);
int checkMPIResult(int result);

void compareExchange(std::vector<int>* local_nums, int i, int j, int dir);
void batcherMerge(std::vector<int>* local_nums, int numProcs, int myRank);
#endif  // TASKS_TASK_3_BONYUK_P_RADIX_BACHER_RADIX_BACHER_H_
