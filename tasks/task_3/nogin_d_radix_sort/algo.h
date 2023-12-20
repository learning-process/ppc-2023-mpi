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

int left(int num);
int right(int num);
int getNumber(int num, int radix);
std::vector<int> randomVector(int sizeVec, int minValue, int maxValue);
std::vector<int> radixStep(const std::vector<int>& vect, int rad);
std::vector<int> radixSort(const std::vector<int>& vect);
std::vector<int> parallelRadixSort(const std::vector<int>& vec);
std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right);

#endif  // TASKS_TASK_3_NOGIN_D_RADIX_SORT_ALGO_H_
