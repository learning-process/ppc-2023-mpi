// Copyright 2023 Veselov Ilya
#include <vector>
#include <boost/serialization/vector.hpp>
#ifndef TASKS_TASK_2_VESELOV_I_STRIP_HOR_MULT_MATRIX_VECTOR_STRIPHORMULTMATRIXVECTOR_H_
#define TASKS_TASK_2_VESELOV_I_STRIP_HOR_MULT_MATRIX_VECTOR_STRIPHORMULTMATRIXVECTOR_H_

std::vector<int> matrix_vector_multiply_par(std::vector<int> A, std::vector<int> B, int rows, int cols);
std::vector<int> matrix_vector_multiply(std::vector<int> A, std::vector<int> x, int rows, int cols);
std::vector<int> randomMatrix(size_t n, size_t m, int minElem = -100, int maxElem = 100);
std::vector<int> randomVector(size_t n, int minElem = -100, int maxElem = 100);

#endif  // TASKS_TASK_2_VESELOV_I_STRIP_HOR_MULT_MATRIX_VECTOR_STRIPHORMULTMATRIXVECTOR_H_
