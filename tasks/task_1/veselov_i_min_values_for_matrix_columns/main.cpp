// Copyright 2023 Veselov Ilya
#include <mpi.h>
#include <gtest/gtest.h>
#include "task_1/veselov_i_min_values_for_matrix_columns/minValForMatrix.h"

TEST(Min_Val_For_Matrix_Col, Test_random) {
    int rank, size;
    const size_t m = 100;
    const size_t n = 100;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> matrix;
    matrix = randomMatrix(n, m);
    std::vector<int> resPar(m);
    MinInColPar(matrix, n, m, resPar);
    if (rank == 0) {
        std::vector<int> res(m);
        MinInCol(matrix, n, m, res);
        ASSERT_EQ(resPar, res);
    }
}

TEST(Min_Val_For_Matrix_Col, Test_random_positive) {
    int rank, size;
    const size_t m = 100;
    const size_t n = 100;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> matrix;
    matrix = randomMatrix(n, m, 1);
    std::vector<int> resPar(m);
    MinInColPar(matrix, n, m, resPar);
    if (rank == 0) {
        std::vector<int> res(m);
        MinInCol(matrix, n, m, res);
        ASSERT_EQ(resPar, res);
    }
}

TEST(Min_Val_For_Matrix_Col, Test_random_negative) {
    int rank, size;
    const size_t m = 100;
    const size_t n = 100;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> matrix;
    matrix = randomMatrix(n, m, -1000, 0);
    std::vector<int> resPar(m);
    MinInColPar(matrix, n, m, resPar);
    if (rank == 0) {
        std::vector<int> res(m);
        MinInCol(matrix, n, m, res);
        ASSERT_EQ(resPar, res);
    }
}

TEST(Min_Val_For_Matrix_Col, Test_identical_elems) {
    int rank, size;
    const size_t m = 40;
    const size_t n = 40;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m, 10));
    std::vector<int> tmp(m);
    std::vector<int> resPar(m);
    MinInColPar(matrix, n, m, resPar);
    if (rank == 0) {
        std::vector<int> res(m);
        MinInCol(matrix, n, m, res);
        ASSERT_EQ(resPar, res);
    }
}

TEST(Min_Val_For_Matrix_Col, Test_several_min) {
    int rank, size;
    const size_t m = 4;
    const size_t n = 4;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    matrix[0] = {1, 5, 8, 0};
    matrix[1] = {2, 25, 48, 2};
    matrix[2] = {4, 4, 4, 4};
    matrix[3] = {3, 101, 3, 3};
    std::vector<int> resPar(m);
    MinInColPar(matrix, n, m, resPar);
    if (rank == 0) {
        std::vector<int> res(m);
        MinInCol(matrix, n, m, res);
        ASSERT_EQ(resPar, res);
    }
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
