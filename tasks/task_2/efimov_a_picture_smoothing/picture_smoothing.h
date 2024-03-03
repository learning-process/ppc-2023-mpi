// Copyright 2024 Efimov Andrey
#ifndef TASKS_TASK_2_EFIMOV_A_PICTURE_SMOOTHING_PICTURE_SMOOTHING_H_
#define TASKS_TASK_2_EFIMOV_A_PICTURE_SMOOTHING_PICTURE_SMOOTHING_H_
#include <mpi.h>
#include <vector>

std::vector<int> seq_smoothing(const std::vector<int> &mat,
int rows, int cols, int koff);
std::vector<int> par_smoothing(const std::vector<int> &Matrix,
int rows, int cols, int koff);
std::vector<int> get_image(const int  rows, const int cols);

#endif  //  TASKS_TASK_2_EFIMOV_A_PICTURE_SMOOTHING_PICTURE_SMOOTHING_H_
