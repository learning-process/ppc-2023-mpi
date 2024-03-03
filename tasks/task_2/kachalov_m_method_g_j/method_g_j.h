// Copyright 2023 Kachalov Mikhail
#ifndef TASKS_TASK_2_KACHALOV_M_METHOD_G_J_METHOD_G_J_H_
#define TASKS_TASK_2_KACHALOV_M_METHOD_G_J_METHOD_G_J_H_

#include <vector>
#include <random>

std::vector<double> CreateMatrixRandom(int size, std::mt19937 *gen);
std::vector<double> sequential_method_g_j(
        const std::vector<double>& unknownsMatrix,
        const std::vector<double>& constants);
std::vector<double> parallel_method_g_j(
    const std::vector<double>& unknownsMatrix,
    const std::vector<double>& constants);

#endif  // TASKS_TASK_2_KACHALOV_M_METHOD_G_J_METHOD_G_J_H_
