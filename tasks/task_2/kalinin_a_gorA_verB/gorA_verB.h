// Copyright 2023 Kalinin Alexandr
#ifndef TASKS_TASK_2_KALININ_A_GORA_VERB_GORA_VERB_H_
#define TASKS_TASK_2_KALININ_A_GORA_VERB_GORA_VERB_H_
#include <mpi.h>

#include <vector>
#include <algorithm>
#include <random>

std::vector<int> getRandomMatrix(int row_count, int column_count);
std::vector<int> getParallelOperations(const std::vector<int>& matrix1,
    const std::vector<int>& matrix2, int row_count_matrix1,
    int column_count_matrix1);
std::vector<int> getSequentialOperations(const std::vector<int>& matrix1,
    const std::vector<int>& matrix2, int row_count_matrix1,
    int column_count_matrix1, int column_count_matrix2);

#endif  // TASKS_TASK_2_KALININ_A_GORA_VERB_GORA_VERB_H_
