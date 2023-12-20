// Copyright 2023 Yurin Andrey
#ifndef TASKS_TASK_3_YURIN_A_SHELL_SORT_SIMPLE_MERGE_SHELL_SORT_SIMPLE_MERGE_H_
#define TASKS_TASK_3_YURIN_A_SHELL_SORT_SIMPLE_MERGE_SHELL_SORT_SIMPLE_MERGE_H_

#include "vector"

bool IsSorted(std::vector<int> array);
std::vector<int> GetRandomVector(int  size, int minValue, int maxValue);
std::vector<int> MergeTwoSortedArrays(std::vector<int> firstArray, std::vector<int> secondArray);
void MergeTwoSortedArrays(std::vector<int>* array, int leftIndex, int midIndex, int rightIndex);
std::vector<int> ShellSortSequential(std::vector<int> array);
std::vector<int> ShellSortParallel(std::vector<int> array, bool combineInOneProcess = false);

#endif  // TASKS_TASK_3_YURIN_A_SHELL_SORT_SIMPLE_MERGE_SHELL_SORT_SIMPLE_MERGE_H_
