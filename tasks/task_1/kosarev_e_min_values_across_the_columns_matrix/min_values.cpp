// Copyright 2023 Kosarev Egor
#include <mpi.h>
#include <random>
#include <climits>
#include <iostream>
#include "task_1/kosarev_e_min_values_across_the_columns_matrix/min_values.h"

std::vector<int> getRandomMatrix(int rows, int cols) {
    std::random_device device;
    std::mt19937 generator(device());
    constexpr int mod = 1000;
    std::vector<int> ans(rows * cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            ans[i * cols + j] = generator() % mod - mod / 2;
        }
    }
    return ans;
}

std::vector<int> GetMinCols(const std::vector<int>& matrix, const int rows, const int cols) {
    std::vector<int> Min(cols, 1e9);

    for (int j = 0; j < rows * cols; j = j + cols) {
        for (int i = 0; i < cols; i++) {
            int index = i + j;
            if (matrix[index] < Min[i]) {
                Min[i] = matrix[index];
            }
        }
    }

    return Min;
}

std::vector<int> GetMinColsParallel(const std::vector<int>& matrix, const int rows, const int cols) {
    int Rows = 0, Cols = 0, numProc, rankProc;
    std::vector<int> Matrix;

    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    if (rankProc == 0) {
        int np = 0;
        if (rows % numProc != 0)
            np = (rows / numProc + 1) * numProc - rows;

        Matrix = std::vector<int>((rows + np) * cols);

        for (int i = 0; i < rows * cols; i++)
            Matrix.at(i) = matrix.at(i);
        for (int i = rows * cols; i < (rows + np) * cols; i++)
            Matrix.at(i) = INT_MAX;

        Rows = rows + np;
        Cols = cols;
    }
    MPI_Bcast(&Rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    Rows = Rows / numProc;
    std::vector<int> Buffer = std::vector<int>(Rows * Cols);

    MPI_Scatter(Matrix.data(), Rows * Cols, MPI_INT, Buffer.data(), Rows * Cols, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> Min(Cols, INT_MAX);
    std::vector<int> tmpMin = GetMinCols(Buffer, Rows, Cols);
    MPI_Reduce(tmpMin.data(), Min.data(), Cols, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    return Min;
}
