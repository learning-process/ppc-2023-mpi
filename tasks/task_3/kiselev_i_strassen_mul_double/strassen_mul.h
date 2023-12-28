// Copyright 2023 Kiselev Igor
#ifndef TASKS_TASK_3_KISELEV_I_STRASSEN_MUL_DOUBLE_STRASSEN_MUL_H_
#define TASKS_TASK_3_KISELEV_I_STRASSEN_MUL_DOUBLE_STRASSEN_MUL_H_
#include <mpi.h>
#include <iostream>
#include <vector>
#include <boost/core/invoke_swap.hpp>

std::vector<std::vector<double>> sumMatrix(const std::vector<std::vector<double>> &sumA,
    const std::vector<std::vector<double>> &sumB, int size, bool sign);
std::vector<std::vector<double>> subMatrix(const std::vector<std::vector<double>> &original,
    int subi, int subj, int size);
std::vector<std::vector<double>> collectMatrix(const std::vector<std::vector<double>> &quarter1,
    const std::vector<std::vector<double>> &quarter2, const std::vector<std::vector<double>> &quarter3,
    const std::vector<std::vector<double>> &quarter4, int size);
std::vector<std::vector<double>> Strassen(const std::vector<std::vector<double>> &StrA,
    const std::vector<std::vector<double>> &StrB, int size);
std::vector<std::vector<double>> SeqMulMatrix(const std::vector<std::vector<double>> &SeqA,
    const std::vector<std::vector<double>> &SeqB, int size);

#endif  // TASKS_TASK_3_KISELEV_I_STRASSEN_MUL_DOUBLE_STRASSEN_MUL_H_
