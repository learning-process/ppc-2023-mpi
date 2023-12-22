// Copyright 2023 Savchuk Anton

#ifndef TASKS_TASK_3_SAVCHUK_A_QUICK_SORT_SIMPLE_MERGE_QUICK_SORT_H_
#define TASKS_TASK_3_SAVCHUK_A_QUICK_SORT_SIMPLE_MERGE_QUICK_SORT_H_

#include <mpi.h>
#include <random>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>

std::pair<int, int> split(int* array, int n);
void recursive_quick_sort(int* array, int n);
void Merge(int* array1, int size1, int size2);
void parallelQuickSort(int* array, size_t n);
bool checking(int* array, int n);

#endif  // TASKS_TASK_3_SAVCHUK_A_QUICK_SORT_SIMPLE_MERGE_QUICK_SORT_H_
