// Copyright 2023 Karagodin Andrey
#ifndef TASKS_TASK_2_KARAGODIN_A_YAKOBI_METHOD_YAKOBI_METHOD_H_
#define TASKS_TASK_2_KARAGODIN_A_YAKOBI_METHOD_YAKOBI_METHOD_H_

#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>

double normalize(const std::vector<double> &v);

std::vector<std::vector<double>> MatrixIncrease(
    const std::vector<std::vector<double>> &m, std::size_t shift);

std::vector<double> operator-(
    const std::vector<double> &a, const std::vector<double> &b);

std::vector<double> VectorIncrease(
    const std::vector<double> &v, std::size_t shift);

std::vector<double> YakobiParallel(
    std::vector<std::vector<double>> a, std::vector<double> b);

std::vector<double> YakobiSequential(
    const std::vector<std::vector<double>>& a,
     const std::vector<double>& b);

#endif  // TASKS_TASK_2_KARAGODIN_A_YAKOBI_METHOD_YAKOBI_METHOD_H_
