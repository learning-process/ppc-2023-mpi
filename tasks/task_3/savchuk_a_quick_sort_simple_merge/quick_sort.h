// Copyright 2023 Savchuk Anton
#ifndef TASKS_TASK_3_SAVCHUK_A_QUICK_SORT_SIMPLE_MERGE_QUICK_SORT_H_
#define TASKS_TASK_3_SAVCHUK_A_QUICK_SORT_SIMPLE_MERGE_QUICK_SORT_H_

#include <random>
#include <vector>
#include <utility>

std::pair<int, int> split(int* arr, int n);
void single_quick_sort(int* arr, int n);
void merge(int* arr1, int size1, int size2);
void parallel_quick_sort(int* arr, size_t n);
bool check(int* arr, int n);
#endif  // TASKS_TASK_3_SAVCHUK_A_QUICK_SORT_SIMPLE_MERGE_QUICK_SORT_H_
