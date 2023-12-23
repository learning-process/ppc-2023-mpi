// Copyright 2023 Nogin Denis

#ifndef TASKS_TASK_3_NOGIN_D_RADIX_SORT_ALGO_H_
#define TASKS_TASK_3_NOGIN_D_RADIX_SORT_ALGO_H_

#include <mpi.h>
#include <iostream>
#include <random>
#include <list>
#include <string>
#include <sstream>
#include <vector>

int getDigit(int num, int place);
int getMax(const std::vector<int>& vec);
std::vector<int> countingSort(const std::vector<int>& vec, int place);
std::vector<int> radixSort(const std::vector<int>& vec);
std::vector<int> parallelRadixSort(const std::vector<int>& vec);
std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right);
std::vector<int> randomVector(int sizeVec, int minValue, int maxValue);

#endif  // TASKS_TASK_3_NOGIN_D_RADIX_SORT_ALGO_H_
