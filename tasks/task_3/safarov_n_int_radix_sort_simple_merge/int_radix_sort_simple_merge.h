// Copyright 2023 Safarov Nurlan
#ifndef TASKS_TASK_3_SAFAROV_N_INT_RADIX_SORT_SIMPLE_MERGE_INT_RADIX_SORT_SIMPLE_MERGE_H_
#define TASKS_TASK_3_SAFAROV_N_INT_RADIX_SORT_SIMPLE_MERGE_INT_RADIX_SORT_SIMPLE_MERGE_H_

#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

void countSortByDigit(int* v, int d, int size);
std::vector<int> generateRandomVector(int size);
std::vector<int> mergeSortedVectors(std::vector<std::vector<int>> m);
std::vector<int> radixSortWithSimpleMergeSequential(std::vector<int> vector);
std::vector<int> radixSortWithSimpleMergeParallel(std::vector<int> vector, int sizeOfVector);

#endif  // TASKS_TASK_3_SAFAROV_N_INT_RADIX_SORT_SIMPLE_MERGE_INT_RADIX_SORT_SIMPLE_MERGE_H_
