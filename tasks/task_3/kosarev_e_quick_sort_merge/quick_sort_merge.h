// Copyright 2023 Kosarev Egor

#ifndef TASKS_TASK_3_KOSAREV_E_QUICK_SORT_MERGE_QUICK_SORT_MERGE_H_
#define TASKS_TASK_3_KOSAREV_E_QUICK_SORT_MERGE_QUICK_SORT_MERGE_H_
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
#endif  // TASKS_TASK_3_KOSAREV_E_QUICK_SORT_MERGE_QUICK_SORT_MERGE_H_
