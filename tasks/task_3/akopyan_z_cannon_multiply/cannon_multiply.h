// Copyright 2023 Akopyan Zal
#ifndef TASKS_TASK_3_AKOPYAN_Z_CANNON_MULTIPLY_CANNON_MULTIPLY_H_
#define TASKS_TASK_3_AKOPYAN_Z_CANNON_MULTIPLY_CANNON_MULTIPLY_H_

#include <cmath>
#include <vector>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

std::vector<std::vector<double>> GetRandomMatrix(int size);
std::vector<std::vector<double>> Matrix_multiply(const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, size_t matrix_dimension);
std::vector<std::vector<double>> Cannon_multiply(const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, size_t mat_size);

#endif  // TASKS_TASK_3_AKOPYAN_Z_CANNON_MULTIPLY_CANNON_MULTIPLY_H_
