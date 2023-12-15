// Copyright 2023 Sadikov Damir
#ifndef TASKS_TASK_3_SADIKOV_D_RADIX_SORT_BATCHER_MERGE_RADIX_SORT_BATCHER_MERGE_H_
#define TASKS_TASK_3_SADIKOV_D_RADIX_SORT_BATCHER_MERGE_RADIX_SORT_BATCHER_MERGE_H_

#include <vector>

std::vector<double> getRandomVector(int n);

void iterativeRadixSort(std::vector<double>& v);

// v.size() == 2 ^ k, k >= 2; use only 2 processes!
void parallelBatcherMergeOfRadixSort(std::vector<double>& v, int n);

#endif  // TASKS_TASK_3_SADIKOV_D_RADIX_SORT_BATCHER_MERGE_RADIX_SORT_BATCHER_MERGE_H_
