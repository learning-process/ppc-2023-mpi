// Copyright 2023 Veselov Ilya
#include <mpi.h>
#include <random>
#include "task_2/veselov_i_strip_hor_mult_matrix_vector/stripHorMultMatrixVector.h"

std::vector<int> matrix_vector_multiply(const std::vector<int>& A, const std::vector<int>& X, int rows, int cols) {
    std::vector<int> res(rows);
    for (int i = 0; i < rows; i++) {
        res[i] = 0;
        for (int j = 0; j < cols; j++)
            res[i] += A[i * cols + j] * X[j];
    }
    return res;
}

std::vector<int> randomMatrix(int rows, int cols, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<int> matrix(rows * cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i * cols + j] = dis(gen);
    return matrix;
}

std::vector<int> matrix_vector_multiply_par(const std::vector<int>& A, std::vector<int> X, int rows, int cols) {
    int r, s;
    std::vector<int> result(rows, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    int chunkSize = rows / s;
    int remainder = rows % s;
    int start = r * chunkSize;
    int end = (r + 1) * chunkSize;
    if (r == s - 1)
        end += remainder;
    std::vector<int> localRes(chunkSize, 0);
    std::vector<int> localMatrix(chunkSize * cols);
    MPI_Scatter(A.data(), chunkSize * cols, MPI_INT, localMatrix.data(), chunkSize * cols, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(X.data(), cols, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = start; i < end; ++i) {
        for (int j = 0; j < cols; ++j) {
            localRes[i - start] += localMatrix[(i - start) * cols + j] * X[j];
        }
    }
    MPI_Gather(localRes.data(), chunkSize, MPI_INT, result.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    return result;
}

std::vector<int> randomVector(int size, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++)
        vec[i] = dis(gen);
    return vec;
}
