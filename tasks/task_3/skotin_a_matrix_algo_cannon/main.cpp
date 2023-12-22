// Copyright 2023 Skotin Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include "task_3/skotin_a_matrix_algo_cannon/matrix_algo_cannon.h"

TEST(Matrix_Cannon_Multiply, Test_Identity_Matrix) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size = 5;
    std::vector<std::vector<double>> a(size, std::vector<double>(size));
    std::vector<std::vector<double>> b(size, std::vector<double>(size));

    if (world_rank == 0) {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++) {
                a[i][j] = i * size + j;
                b[i][j] = (i == j) ? 1 : 0;
            }
    }

    std::vector<std::vector<double>> res = CannonMultiply(a, b, size);
    if (world_rank == 0) {
        ASSERT_EQ(a, res);
    }
}

TEST(Matrix_Cannon_Multiply, Test_Zero_Matrix) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size = 4;
    std::vector<std::vector<double>> a(size, std::vector<double>(size, 0));
    std::vector<std::vector<double>> b(size, std::vector<double>(size, 0));

    std::vector<std::vector<double>> res = CannonMultiply(a, b, size);
    if (world_rank == 0) {
        ASSERT_EQ(a, res);
    }
}

TEST(Matrix_Cannon_Multiply, Test_Random_Matrix) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size = 6;
    std::vector<std::vector<double>> a;
    std::vector<std::vector<double>> b;

    if (world_rank == 0) {
        a = GetMatrixRandom(size);
        b = GetMatrixRandom(size);
    }

    std::vector<std::vector<double>> res_par = CannonMultiply(a, b, size);
    if (world_rank == 0) {
        std::vector<std::vector<double>> res_seq = MatrixMultiply(a, b, size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                ASSERT_NEAR(res_seq[i][j], res_par[i][j], 1e-5);
            }
        }
    }
}

TEST(Matrix_Cannon_Multiply, Test_Small_Matrix) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size = 2;
    std::vector<std::vector<double>> a = {{1, 2}, {3, 4}};
    std::vector<std::vector<double>> b = {{2, 0}, {1, 2}};

    std::vector<std::vector<double>> expected = {{4, 4}, {10, 8}};
    std::vector<std::vector<double>> res = CannonMultiply(a, b, size);
    if (world_rank == 0) {
        ASSERT_EQ(expected, res);
    }
}

TEST(Matrix_Cannon_Multiply, Test_Large_Matrix) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int size = 8;
    std::vector<std::vector<double>> a;
    std::vector<std::vector<double>> b;

    if (world_rank == 0) {
        a = GetMatrixRandom(size);
        b = GetMatrixRandom(size);
    }

    std::vector<std::vector<double>> res_par = CannonMultiply(a, b, size);
    if (world_rank == 0) {
        std::vector<std::vector<double>> res_seq = MatrixMultiply(a, b, size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                ASSERT_NEAR(res_seq[i][j], res_par[i][j], 1e-5);
            }
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
