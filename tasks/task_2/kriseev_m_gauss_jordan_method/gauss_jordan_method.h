// Copyright 2023 Kriseev Mikhail

#ifndef TASKS_TASK_2_KRISEEV_M_GAUSS_JORDAN_METHOD_GAUSS_JORDAN_METHOD_H_
#define TASKS_TASK_2_KRISEEV_M_GAUSS_JORDAN_METHOD_GAUSS_JORDAN_METHOD_H_
#include <vector>

std::vector<double> gaussJordanMethodSequential(
        const std::vector<double>& unknownsMatrix,
        const std::vector<double>& constants);
std::vector<double> gaussJordanMethodParallel(
    const std::vector<double>& unknownsMatrix,
    const std::vector<double>& constants);

#endif  // TASKS_TASK_2_KRISEEV_M_GAUSS_JORDAN_METHOD_GAUSS_JORDAN_METHOD_H_
