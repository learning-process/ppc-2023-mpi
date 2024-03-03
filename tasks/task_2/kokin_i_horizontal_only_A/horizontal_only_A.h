// Copyright 2024 Kokin Ivan
#ifndef TASKS_TASK_2_KOKIN_I_HORIZONTAL_ONLY_A_HORIZONTAL_ONLY_A_H_
#define TASKS_TASK_2_KOKIN_I_HORIZONTAL_ONLY_A_HORIZONTAL_ONLY_A_H_

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> MultiplicateMatrix(
    std::vector<int>* A, std::vector<int>* B,
    std::vector<int>* C, int max_size, int count);
std::vector<int> MultiplicateMatrixParalell(
    std::vector<int>* A, std::vector<int>* B,
    std::vector<int>* C, int max_size);

#endif  // TASKS_TASK_2_KOKIN_I_HORIZONTAL_ONLY_A_HORIZONTAL_ONLY_A_H_
