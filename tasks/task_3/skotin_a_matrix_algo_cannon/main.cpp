// Copyright 2023 Skotin Alexander
#include <gtest/gtest.h>
#include "task_3/skotin_a_matrix_algo_cannon/matrix_algo_cannon.h"

TEST(Matrix_Cannon_Multiply, Test_Identity_Matrix) {
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int size = 3;
    std::vector<std::vector<double>> a = GetMatrixRandom(size);
    std::vector<std::vector<double>> b(size, std::vector<double>(size, 0));
    for (int i = 0; i < size; i++) {
        b[i][i] = 1;
    }
    std::vector<std::vector<double>> res = CannonMultiply(a, b, size);
    if (world_rank == 0) {
        ASSERT_EQ(a, res);
    }
}

TEST(Matrix_Cannon_Multiply, Test_Zero_Matrix) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size = 3;
    std::vector<std::vector<double>> a = GetMatrixRandom(size);
    std::vector<std::vector<double>> b(size, std::vector<double>(size, 0));
    std::vector<std::vector<double>> res = CannonMultiply(a, b, size);
    std::vector<std::vector<double>> expected(size,
        std::vector<double>(size, 0));
    if (world_rank == 0) {
        ASSERT_EQ(expected, res);
    }
}

TEST(Matrix_Cannon_Multiply, Test_Random_Matrix) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size = 3;
    std::vector<std::vector<double>> a = GetMatrixRandom(size);
    std::vector<std::vector<double>> b = GetMatrixRandom(size);
    std::vector<std::vector<double>> res = CannonMultiply(a, b, size);
    std::vector<std::vector<double>> expected = MatrixMultiply(a, b, size);
    if (world_rank == 0) {
        ASSERT_EQ(expected, res);
    }
}

TEST(Matrix_Cannon_Multiply, Test_Square_Matrix) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size = 3;
    std::vector<std::vector<double>> a(size, std::vector<double>(size, 2));
    std::vector<std::vector<double>> b(size, std::vector<double>(size, 2));
    std::vector<std::vector<double>> res = CannonMultiply(a, b, size);
    std::vector<std::vector<double>> expected = MatrixMultiply(a, b, size);
    if (world_rank == 0) {
        ASSERT_EQ(expected, res);
    }
}

TEST(Matrix_Cannon_Multiply, Test_Large_Matrix) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size = 6;
    std::vector<std::vector<double>> a = GetMatrixRandom(size);
    std::vector<std::vector<double>> b = GetMatrixRandom(size);
    std::vector<std::vector<double>> res = CannonMultiply(a, b, size);
    std::vector<std::vector<double>> expected = MatrixMultiply(a, b, size);
    if (world_rank == 0) {
        ASSERT_EQ(expected, res);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
