// Copyright 2023 Kostanyan Arsen
#ifndef TASKS_TASK_2_KOSTANYAN_A_PICTURE_SMOOTHING_ALG_H_
#define TASKS_TASK_2_KOSTANYAN_A_PICTURE_SMOOTHING_ALG_H_
#include <mpi.h>
#include <vector>

std::vector<int> getImg(const int  rows, const int cols);
std::vector<int> SeqSmoothing(const std::vector<int> &mat, int rows, int cols, int koff);
std::vector<int> ParSmoothing(const std::vector<int> &Matrix, int rows, int cols, int koff);

#endif  //  TASKS_TASK_2_KOSTANYAN_A_PICTURE_SMOOTHING_ALG_H_
