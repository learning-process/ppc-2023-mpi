// Copyright 2023 Kiselev Igor
#ifndef TASKS_TASK_2_KISELEV_I_MUL_TAPE_MATRIX_MUL_TAPE_MATRIX_H_
#define TASKS_TASK_2_KISELEV_I_MUL_TAPE_MATRIX_MUL_TAPE_MATRIX_H_
#include <mpi.h>
#include <vector>

std::vector<int> SeqMulMatrix(const std::vector<int>& a, const std::vector<int>& b, int an, int am, int bm);
std::vector<int> ParMulMatrix(std::vector<int> *SMM, std::vector<int> *PMM, int an, int am);
#endif  // TASKS_TASK_2_KISELEV_I_MUL_TAPE_MATRIX_MUL_TAPE_MATRIX_H_
