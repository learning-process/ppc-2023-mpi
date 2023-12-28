// Copyright 2023 Troitskiy Alexandr

#ifndef TASKS_TASK_3_TROITSKIY_A_DOUBLE_RADIX_SORT_H_
#define TASKS_TASK_3_TROITSKIY_A_DOUBLE_RADIX_SORT_H_

#include <mpi.h>
#include <climits>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>

std::vector<double> DoubleRadixSortSeq(std::vector<double> vec);
std::vector<double> generateRandomDoubleVector(int n, double min, double max);
void SortByCount(double* input, double* temp, int byte_num, int n);
std::vector<double> DoubleRadixSortParallel(std::vector<double> vec);
std::vector<double> MergeVectors(const std::vector<double>& vec1, const std::vector<double>& vec2);
#endif  // TASKS_TASK_3_TROITSKIY_A_DOUBLE_RADIX_SORT_H_
