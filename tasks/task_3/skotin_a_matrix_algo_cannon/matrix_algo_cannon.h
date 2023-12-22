// Copyright 2023 Skotin Alexander
#ifndef TASKS_TASK_3_SKOTIN_A_MATRIX_ALGO_CANNON_MATRIX_ALGO_CANNON_H_
#define TASKS_TASK_3_SKOTIN_A_MATRIX_ALGO_CANNON_MATRIX_ALGO_CANNON_H_

#include <mpi.h>
#include <vector>
#include <cmath>

std::vector<std::vector<double>> MatrixMultiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, int matrix_dimension);
std::vector<std::vector<double>> CannonMultiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, int matrix_dimension);
std::vector<std::vector<double>> GetMatrixRandom(int size);

#endif  // TASKS_TASK_3_SKOTIN_A_MATRIX_ALGO_CANNON_MATRIX_ALGO_CANNON_H_
