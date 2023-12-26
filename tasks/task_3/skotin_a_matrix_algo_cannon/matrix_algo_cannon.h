// Copyright 2023 Skotin Alexander
#ifndef TASKS_TASK_3_SKOTIN_A_MATRIX_ALGO_CANNON_MATRIX_ALGO_CANNON_H_
#define TASKS_TASK_3_SKOTIN_A_MATRIX_ALGO_CANNON_MATRIX_ALGO_CANNON_H_

#include <mpi.h>
#include <vector>
#include <cmath>

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

std::vector<std::vector<double>> MatrixMultiply(
    const std::vector<std::vector<double>>& matrix_1,
    const std::vector<std::vector<double>>& matrix_2, int m_size);
std::vector<std::vector<double>> CannonMultiply(
    const std::vector<std::vector<double>>& matrix_1,
    const std::vector<std::vector<double>>& matrix_2, size_t m_size);
std::vector<std::vector<double>> GetMatrixRandom(int size);

#endif  // TASKS_TASK_3_SKOTIN_A_MATRIX_ALGO_CANNON_MATRIX_ALGO_CANNON_H_
