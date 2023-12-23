// Copyright 2023 Kostanyan Arsen
#ifndef TASKS_TASK_1_KOSTANYAN_A_SUM_VALUES_BY_ROWS_MATRIX_ALG_H_
#define TASKS_TASK_1_KOSTANYAN_A_SUM_VALUES_BY_ROWS_MATRIX_ALG_H_
#include <mpi.h>
#include <vector>
#include <ctime>
#include <stdexcept>
#include <random>

std::vector<int> RandomMatrix(int rows, int cols);

std::vector<int> calculateRowSums(const std::vector<int>& matrix, int rows, int cols);

std::vector<int> ParallelProcessing(const std::vector<int>& matrix, int rows, int cols);
#endif  //  TASKS_TASK_1_KOSTANYAN_A_SUM_VALUES_BY_ROWS_MATRIX_ALG_H_
