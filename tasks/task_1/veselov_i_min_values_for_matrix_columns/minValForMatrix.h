// Copyright 2023 Veselov Ilya
#include <vector>
#ifndef TASKS_TASK_1_VESELOV_I_MIN_VALUES_FOR_MATRIX_COLUMNS_MINVALFORMATRIX_H_
#define TASKS_TASK_1_VESELOV_I_MIN_VALUES_FOR_MATRIX_COLUMNS_MINVALFORMATRIX_H_

void MinInCol(const std::vector<std::vector<int>>& matrix, size_t n, size_t m, std::vector<int> res);
void MinInColPar(const std::vector<std::vector<int>>& matrix, size_t n, size_t m, std::vector<int> res);
std::vector<std::vector<int>> randomMatrix(size_t n, size_t m, int minElem = -100, int maxElem = 100);

#endif  // TASKS_TASK_1_VESELOV_I_MIN_VALUES_FOR_MATRIX_COLUMNS_MINVALFORMATRIX_H_"
