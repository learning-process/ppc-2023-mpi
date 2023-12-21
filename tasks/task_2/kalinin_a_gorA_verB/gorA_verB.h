// Copyright 2023 Kalinin Alexandr
#ifndef TASKS_TASK_1_KALININ_A_LEX_STRINGS_LEX_STRINGS_H_
#define TASKS_TASK_1_KALININ_A_LEX_STRINGS_LEX_STRINGS_H_
#include <mpi.h>

#include <vector>
#include <algorithm>
#include <random>

std::vector<int> getRandomMatrix(int row_count,
    int column_count);
std::vector<int> getParallelOperations(const
    std::vector<int>& matrix1,
    const std::vector<int>& matrix2,
    int row_count_matrix1,
    int column_count_matrix1);

#endif  // TASKS_TASK_1_KALININ_A_LEX_STRINGS_LEX_STRINGS_H_
