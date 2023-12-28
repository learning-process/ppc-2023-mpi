// Copyright 2023 Bakhtiarov Alexander
#include <gtest/gtest.h>
#include "task_3/bakhtiarov_a_multiplication_of_sparse_crs_matrices/multiplication_of_sparse_crs_matrices.h"

TEST(multiplication_of_sparse_crs_matrices, Test_difficult_matrix) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    CRS leftMatrix;

    leftMatrix.values = {1, 2, 7};
    leftMatrix.col_indices = {0, 1, 1};
    leftMatrix.row_ptr = {0, 1, 2, 2, 3};
    leftMatrix.row = 4;
    leftMatrix.col = 2;

    CRS rightMatrix;

    rightMatrix.values = {3, 4};
    rightMatrix.col_indices = {0, 2};
    rightMatrix.row_ptr = {0, 1, 2};
    rightMatrix.row = 2;
    rightMatrix.col = 3;

    CRS ansMatrix = multiMatrix(leftMatrix, rightMatrix, numProc, rankProc);

    if (rankProc == 0) {
        std::vector<double> keyMatrix = {3, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 28};
        CRS key(keyMatrix, 4, 3);
        ASSERT_EQ(ansMatrix.values, key.values);
        ASSERT_EQ(ansMatrix.col_indices, key.col_indices);
        ASSERT_EQ(ansMatrix.row_ptr, key.row_ptr);
    }
}

TEST(multiplication_of_sparse_crs_matrices, Test_squware_matrix) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<double> lMatrix = {0, 7, 0, 0, 6, 5, 3, 0, 0};
    CRS leftMatrix(lMatrix, 3, 3);

    std::vector<double> rMatrix = {0, 0, 8, 1, 0, 0, 0, 9, 0};
    CRS rightMatrix(rMatrix, 3, 3);

    CRS ansMatrix = multiMatrix(leftMatrix, rightMatrix, numProc, rankProc);

    if (rankProc == 0) {
        std::vector<double> keyMatrix = {7, 0, 0, 6, 45, 0, 0, 0, 24};
        CRS key(keyMatrix, 3, 3);
        ASSERT_EQ(ansMatrix.values, key.values);
        ASSERT_EQ(ansMatrix.col_indices, key.col_indices);
        ASSERT_EQ(ansMatrix.row_ptr, key.row_ptr);
    }
}

TEST(multiplication_of_sparse_crs_matrices, Test_big_squware_matrix) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    CRS leftMatrix;

    leftMatrix.values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    leftMatrix.col_indices = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    leftMatrix.row_ptr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    leftMatrix.row = 10;
    leftMatrix.col = 10;

    CRS rightMatrix;

    rightMatrix.values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    rightMatrix.col_indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    rightMatrix.row_ptr = {0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    rightMatrix.row = 10;
    rightMatrix.col = 10;

    CRS ansMatrix = multiMatrix(leftMatrix, rightMatrix, numProc, rankProc);

    if (rankProc == 0) {
        std::vector<double> keyMatrix;
        for (int i = 1; i < 11; ++i) {
            for (int j = 1; j < 11; ++j) {
                keyMatrix.push_back(i*j);
            }
        }
        CRS key(keyMatrix, 10, 10);
        ASSERT_EQ(ansMatrix.values, key.values);
        ASSERT_EQ(ansMatrix.col_indices, key.col_indices);
        ASSERT_EQ(ansMatrix.row_ptr, key.row_ptr);
    }
}

TEST(multiplication_of_sparse_crs_matrices, Test_zero_matrix) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    std::vector<double> val = {};
    CRS leftMatrix(val, 10, 10);
    CRS rightMatrix(val, 10, 10);

    CRS ansMatrix = multiMatrix(leftMatrix, rightMatrix, numProc, rankProc);

    if (rankProc == 0) {
        CRS key(val, 10, 10);
        ASSERT_EQ(ansMatrix.values, key.values);
        ASSERT_EQ(ansMatrix.col_indices, key.col_indices);
        ASSERT_EQ(ansMatrix.row_ptr, key.row_ptr);
    }
}

TEST(multiplication_of_sparse_crs_matrices, Test_big_zero_matrix) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    std::vector<double> val = {};
    CRS leftMatrix(val, 10, 10);
    CRS rightMatrix(val, 10, 10);

    CRS ansMatrix = multiMatrix(leftMatrix, rightMatrix, numProc, rankProc);

    if (rankProc == 0) {
        CRS key(val, 10, 10);
        ASSERT_EQ(ansMatrix.values, key.values);
        ASSERT_EQ(ansMatrix.col_indices, key.col_indices);
        ASSERT_EQ(ansMatrix.row_ptr, key.row_ptr);
    }
}

int main(int argc, char **argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
