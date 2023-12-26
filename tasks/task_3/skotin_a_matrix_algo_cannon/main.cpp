// Copyright 2023 Skotin Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include "task_3/skotin_a_matrix_algo_cannon/matrix_algo_cannon.h"

TEST(CannonMatrixMultiply, TestIdentity) {
    int size = 7;
    std::vector<std::vector<double>> A = GetMatrixRandom(size);
    std::vector<std::vector<double>> I(size, std::vector<double>(size, 0));
    for (int i = 0; i < size; i++) {
        I[i][i] = 1;
    }
    std::vector<std::vector<double>> result = CannonMultiply(A, I, size);
    ASSERT_EQ(A, result);
}

TEST(CannonMatrixMultiply, TestZero) {
    int size = 7;
    std::vector<std::vector<double>> A = GetMatrixRandom(size);
    std::vector<std::vector<double>> Z(size, std::vector<double>(size, 0));
    std::vector<std::vector<double>> result = CannonMultiply(A, Z, size);
    ASSERT_EQ(Z, result);
}

TEST(CannonMatrixMultiply, TestRandom) {
    int size = 10;
    std::vector<std::vector<double>> A = GetMatrixRandom(size);
    std::vector<std::vector<double>> B = GetMatrixRandom(size);
    std::vector<std::vector<double>> result = CannonMultiply(A, B, size);
    std::vector<std::vector<double>> expected = MatrixMultiply(A, B, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            ASSERT_NEAR(expected[i][j], result[i][j], 1e-9);
        }
    }
}

TEST(CannonMatrixMultiply, TestSquare) {
    int size = 6;
    std::vector<std::vector<double>> A = GetMatrixRandom(size);
    std::vector<std::vector<double>> B = GetMatrixRandom(size);
    std::vector<std::vector<double>> result = CannonMultiply(A, B, size);
    std::vector<std::vector<double>> expected = MatrixMultiply(A, B, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            ASSERT_NEAR(expected[i][j], result[i][j], 1e-9);
        }
    }
}

TEST(CannonMatrixMultiply, TestLarge) {
    int size = 15;
    std::vector<std::vector<double>> A = GetMatrixRandom(size);
    std::vector<std::vector<double>> B = GetMatrixRandom(size);
    std::vector<std::vector<double>> result = CannonMultiply(A, B, size);
    std::vector<std::vector<double>> expected = MatrixMultiply(A, B, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            ASSERT_NEAR(expected[i][j], result[i][j], 1e-9);
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
