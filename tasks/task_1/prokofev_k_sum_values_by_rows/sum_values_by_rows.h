// Copyright 2023 Prokofev Kirill
#ifndef TASKS_TASK_1_PROKOFEV_K_SUM_VALUES_BY_ROWS_SUM_VALUES_BY_ROWS_H_
#define TASKS_TASK_1_PROKOFEV_K_SUM_VALUES_BY_ROWS_SUM_VALUES_BY_ROWS_H_

#include <mpi.h>
#include <random>
#include <vector>

int Random(int low, int high);
std::vector<int>GenerateRandomMatrix(int n, int m);
std::vector<int>ParallSumValuesByRows(const std::vector<int>& matrix, int n, int m);
std::vector<int>SeqSumValuesByRows(const std::vector<int>& matrix, int n, int m);
#endif  // TASKS_TASK_1_PROKOFEV_K_SUM_VALUES_BY_ROWS_SUM_VALUES_BY_ROWS_H_
