// Copyright 2023 Veselov Ilya
#include <vector>
#ifndef TASKS_TASK_2_VESELOV_I_STRIP_HOR_MULT_MATRIX_VECTOR_STRIPHORMULTMATRIXVECTOR_H_
#define TASKS_TASK_2_VESELOV_I_STRIP_HOR_MULT_MATRIX_VECTOR_STRIPHORMULTMATRIXVECTOR_H_

std::vector<int> matrix_vector_multiply_par(const std::vector<int>& A, std::vector<int> X, int rows, int cols);
std::vector<int> matrix_vector_multiply(const std::vector<int>& A, const std::vector<int>& X, int rows, int cols);
std::vector<int> randomVector(int size, int minElem = -100, int maxElem = 100);
std::vector<int> randomMatrix(int rows, int cols, int minElem = -100, int maxElem = 100);

#endif  // TASKS_TASK_2_VESELOV_I_STRIP_HOR_MULT_MATRIX_VECTOR_STRIPHORMULTMATRIXVECTOR_H_
