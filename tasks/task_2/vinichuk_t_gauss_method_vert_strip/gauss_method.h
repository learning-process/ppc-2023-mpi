// Copyright 2023 Vinichuk Timofey
#ifndef TASKS_TASK_2_VINICHUK_T_GAUSS_METHOD_VERT_STRIP_GAUSS_METHOD_H_
#define TASKS_TASK_2_VINICHUK_T_GAUSS_METHOD_VERT_STRIP_GAUSS_METHOD_H_
#include <vector>

std::vector<double> SeqMethodGaussian(const std::vector<double>& matrix, int h);
std::vector<double> ParallelMethodGaussian(const std::vector<double>& matrix,
    int n);

#endif  //  TASKS_TASK_2_VINICHUK_T_GAUSS_METHOD_VERT_STRIP_GAUSS_METHOD_H_
