// Copyright 2023 Musaev Ilgar
#ifndef TASKS_TASK_3_MUSAEV_I_MATRIX_DOUBLE_CRS_MATRIX_DOUBLE_CRS_H_
#define TASKS_TASK_3_MUSAEV_I_MATRIX_DOUBLE_CRS_MATRIX_DOUBLE_CRS_H_

#include <tbb/tbb.h>
#include <random>
#include <ctime>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <cstring>
#include <complex>

struct Matrix {
    int rows, cols;
    std::vector<std::complex<int>> val;
    Matrix(int rows, int cols) : rows(rows), cols(cols), val(cols * rows) {}
    ~Matrix() = default;
};

struct MatrixCRS {
    int rows, cols;
    std::vector<std::complex<int>> val;
    std::vector<int> cols_pos;
    std::vector<int> ptrs;
    MatrixCRS() = default;
    ~MatrixCRS() = default;
};

MatrixCRS generateRandomCRSMat(int rows, int cols);
Matrix generateRandomMat(int rows, int cols);
MatrixCRS convert(const Matrix &inMat);
MatrixCRS transp(const MatrixCRS &inMat);
MatrixCRS matrixCRSMult(const MatrixCRS &first, const MatrixCRS &second);
MatrixCRS matrixCRSMultTBB(const MatrixCRS &first, const MatrixCRS &second);
Matrix matrixMult(const Matrix &first, const Matrix &second);
void print(const MatrixCRS &in);

#endif  // TASKS_TASK_3_MUSAEV_I_MATRIX_DOUBLE_CRS_MATRIX_DOUBLE_CRS_H_
