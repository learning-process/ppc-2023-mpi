// Copyright 2023 Savchuk Anton
#ifndef TASKS_TASK_1_SAVCHUK_A_MATRIX_MIN_BY_COLUMNS_MATRIX_MIN_COLUMNS_H_
#define TASKS_TASK_1_SAVCHUK_A_MATRIX_MIN_BY_COLUMNS_MATRIX_MIN_COLUMNS_H_

#include <mpi.h>
#include <vector>


std::vector<int> generateMatrix(int n, int m);
std::vector<int> transposeMatrix(const std::vector<int>& matrix, int n, int m);
int getMinInSequence(const std::vector<int>& sec);
std::vector<int> getColumnMinParallel(const std::vector<int>& matrix,
                                        int n, int m);

#endif  // TASKS_TASK_1_SAVCHUK_A_MATRIX_MIN_BY_COLUMNS_MATRIX_MIN_COLUMNS_H_
