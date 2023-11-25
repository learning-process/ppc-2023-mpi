// Copyright 2023 Sadikov Damir
#ifndef TASKS_TASK_3_SADIKOV_D_RADIX_SORT_BATCHER_MERGE_RADIX_SORT_BATCHER_MERGE_H_
#define TASKS_TASK_3_SADIKOV_D_RADIX_SORT_BATCHER_MERGE_RADIX_SORT_BATCHER_MERGE_H_

#include <vector>

std::vector<double> getRandomVector(int n);
std::vector<double> getRandomMatrix(int n);
double NormOfDifference(const std::vector<double>& a, const std::vector<double>& b);
std::vector<double> SequentialIter(const std::vector<double>& A, const std::vector<double>& b, int n);
std::vector<double> ParallelIter(const std::vector<double>& A, const std::vector<double>& b, int n);

#endif  // TASKS_TASK_3_SADIKOV_D_RADIX_SORT_BATCHER_MERGE_RADIX_SORT_BATCHER_MERGE_H_
