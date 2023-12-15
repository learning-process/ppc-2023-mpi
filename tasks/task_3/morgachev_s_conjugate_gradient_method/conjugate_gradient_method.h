// Copyright 2023 Morgachev Stepan
#ifndef TASKS_TASK_3_MORGACHEV_S_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_METHOD_H_
#define TASKS_TASK_3_MORGACHEV_S_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_METHOD_H_

#include <mpi.h>
#include <vector>
#include <cmath>
#include <iostream>

std::vector<double> fillVectorRandomNumbers(size_t size, int min, int max);
std::vector<double> fillMatrixRandomNumbers(size_t size, int min, int max);
double scalarMultiply(const std::vector<double>& vectorX,
    const std::vector<double>& vectorY);
std::vector<double> multiplyMatrixToVector(const std::vector<double>& matrix,
    const std::vector<double>& vector);
std::vector<double> serialConjugateGradient(const std::vector<double>& matrix,
    const std::vector<double>& vector, size_t size);
std::vector<double> parallelConjugateGradient(const std::vector<double>& matrix,
    const std::vector<double>& vector, size_t size);

#endif  // TASKS_TASK_3_MORGACHEV_S_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_METHOD_H_
