// Copyright 2023 Veselov Ilya
#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include "task_2/veselov_i_strip_hor_mult_matrix_vector/stripHorMultMatrixVector.h"

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t rows = 4, cols = 4;
    std::vector<int> matrix(rows * cols), vec(cols), resPar(rows);
    if (rank == 0) {
        matrix = randomMatrix(rows, cols);
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (rank == 0) {
        std::vector<int> res(rows);
        res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random_positive) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t rows = 8, cols = 8;
    std::vector<int> matrix(rows * cols), vec(cols), resPar(rows);
    if (rank == 0) {
        matrix = randomMatrix(rows, cols, 1);
        vec = randomVector(cols, 1);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (rank == 0) {
        std::vector<int> res(rows);
        res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random_negative) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t rows = 8, cols = 8;
    std::vector<int> matrix(rows * cols), vec(cols), resPar(rows);
    if (rank == 0) {
        matrix = randomMatrix(rows, cols, -1000, 0);
        vec = randomVector(cols, -1000, 0);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (rank == 0) {
        std::vector<int> res(rows);
        res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_null_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t rows = 10, cols = 10;
    std::vector<int> matrix(rows * cols, 0), vec(cols), resPar(rows);
    if (rank == 0) {
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (rank == 0) {
        std::vector<int> res(rows);
        res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_matrix_diag) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t rows = 16, cols = 16;
    std::vector<int> matrix(rows * cols), vec(cols), resPar(rows);
    if (rank == 0) {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                if (i == j)
                    matrix[i * rows + j] = 1;
            }
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (rank == 0) {
        std::vector<int> res(rows);
        res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int result = 0, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (rank != 0)
        delete listeners.Release(listeners.default_result_printer());
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
