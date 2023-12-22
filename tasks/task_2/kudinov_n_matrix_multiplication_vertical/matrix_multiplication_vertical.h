// Copyright 2023 Kudinov Nikita
#ifndef TASKS_TASK_2_KUDINOV_N_MATRIX_MULTIPLICATION_VERTICAL_MATRIX_MULTIPLICATION_VERTICAL_H_
#define TASKS_TASK_2_KUDINOV_N_MATRIX_MULTIPLICATION_VERTICAL_MATRIX_MULTIPLICATION_VERTICAL_H_

#include <vector>

template <class T>
using Vector = std::vector<T>;

template <class T>
using Matrix = std::vector<std::vector<T>>;

void print_matrix(const Matrix<double>& matrix);
void print_vector(const Vector<double>& vector);

Matrix<double> matrix_get_submatrix(
    const Matrix<double>& matrix,
    const std::size_t y,
    const std::size_t x,
    const std::size_t height,
    const std::size_t width);

Vector<double> multiply_matrices_sequential(
    const Matrix<double>& matrix,
    const Vector<double>& vector);

Vector<double> multiply_matrices_parallel(
    const Matrix<double>& matrix,
    const Vector<double>& vector);

#endif  // TASKS_TASK_2_KUDINOV_N_MATRIX_MULTIPLICATION_VERTICAL_MATRIX_MULTIPLICATION_VERTICAL_H_
