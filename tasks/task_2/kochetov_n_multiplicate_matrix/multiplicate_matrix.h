// Copyright 2023 Kochetov Nikolay
#ifndef TASKS_TASK_2_KOCHETOV_N_MULTIPLICATE_MATRIX_MULTIPLICATE_MATRIX_H_
#define TASKS_TASK_2_KOCHETOV_N_MULTIPLICATE_MATRIX_MULTIPLICATE_MATRIX_H_

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> MultiplicateMatrix(
    std::vector<int>* A, std::vector<int>* B,
    std::vector<int>* C, int max_size, int count);
std::vector<int> MultiplicateMatrixP(
    std::vector<int>* A, std::vector<int>* B,
    std::vector<int>* C, int max_size);

#endif  // TASKS_TASK_2_KOCHETOV_N_MULTIPLICATE_MATRIX_MULTIPLICATE_MATRIX_H_
