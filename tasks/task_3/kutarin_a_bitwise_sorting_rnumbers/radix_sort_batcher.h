// Copyright 2023 Kutarin Aleksandr
#ifndef TASKS_TASK_3_KUTARIN_A_BITWISE_SORTING_RNUMBERS_RADIX_SORT_BATCHER_H_
#define TASKS_TASK_3_KUTARIN_A_BITWISE_SORTING_RNUMBERS_RADIX_SORT_BATCHER_H_

#include <vector>

std::vector<double> getRandomVector(int n);
std::vector<double> iterativeRadixSort(std::vector<double> v);
std::vector<double> parallelBatcherRadixSort(std::vector<double> v, int n);

#endif  // TASKS_TASK_3_KUTARIN_A_BITWISE_SORTING_RNUMBERS_RADIX_SORT_BATCHER_H_
