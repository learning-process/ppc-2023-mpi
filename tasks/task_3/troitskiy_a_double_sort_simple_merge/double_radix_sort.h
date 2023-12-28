// Copyright 2023 Troitskiy Alexandr

#ifndef TASKS_TASK_3_TROITSKIY_A_DOUBLE_SORT_SIMPLE_MERGE_DOUBLE_RADIX_SORT_H_
#define TASKS_TASK_3_TROITSKIY_A_DOUBLE_SORT_SIMPLE_MERGE_DOUBLE_RADIX_SORT_H_

#include <mpi.h>
#include <climits>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>

std::vector<double> SequenceRadixSortDouble(std::vector<double> a , int n);
std::vector<double> getRandomInput(int n, double fMin, double fMax);
void CountingSort(double* inp, double* tmp, int byteNum, int n);
std::vector<double> ParallelRadixSortDouble(std::vector<double> a, int n);
std::vector<double> merge(const std::vector<double>& a, const std::vector<double>& b);
#endif  // TASKS_TASK_3_TROITSKIY_A_DOUBLE_SORT_SIMPLE_MERGE_DOUBLE_RADIX_SORT_H_
