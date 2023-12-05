// Copyright 2023 Ivanchenko Aleksei
#ifndef TASKS_TASK_1_IVANCHENKO_A_MAX_ACROSS_COLUMNS_MAX_ACROSS_COLUMNS_H_
#define TASKS_TASK_1_IVANCHENKO_A_MAX_ACROSS_COLUMNS_MAX_ACROSS_COLUMNS_H_

#include <vector>

std::vector<int> getRandomMatrix(size_t rows, size_t columns, int minElem = -100, int maxElem = 1000);
std::vector<int> getMaxSequentional(const std::vector<int>& matrix, size_t rows, size_t columns);
std::vector<int> getMaxParallel(const std::vector<int>& matrix, size_t rows, size_t columns);

#endif  // TASKS_TASK_1_IVANCHENKO_A_MAX_ACROSS_COLUMNS_MAX_ACROSS_COLUMNS_H_