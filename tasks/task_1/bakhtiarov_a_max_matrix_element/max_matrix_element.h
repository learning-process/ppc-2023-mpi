// Copyright 2023 Bakhtiarov Alexander
#ifndef TASKS_TASK_1_BAKHTIAROV_A_MAX_MATRIX_ELEMENT_MAX_MATRIX_ELEMENT_H_
#define TASKS_TASK_1_BAKHTIAROV_A_MAX_MATRIX_ELEMENT_MAX_MATRIX_ELEMENT_H_
#include <vector>
#include <string>
std::vector<int> generate_random_Matrix(int rows, int cols);
int findMaxOfMatrix(const std::vector<int>& global_matrix);
int findMaxOfMatrixParallel(const std::vector<int>& global_matrix,
    int rows, int cols);
#endif  // TASKS_TASK_1_BAKHTIAROV_A_MAX_MATRIX_ELEMENT_MAX_MATRIX_ELEMENT_H_
