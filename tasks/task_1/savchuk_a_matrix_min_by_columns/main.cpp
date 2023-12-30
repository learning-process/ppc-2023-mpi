// Copyright 2023 Savchuk Anton
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_min_columns.h"

TEST(matrix_operations, transpose) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<int> matrix = { 1, 2, 3, 4, 5, 6, 7, 8 };
        std::vector<int> check = { 1, 5, 2, 6, 3, 7, 4, 8 };
        std::vector<int> t = transposeMatrix(matrix, 2, 4);

        ASSERT_EQ(t, check);
    }
}

TEST(matrix_operations, test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;
    int n = 2;
    int m = 4;
    if (rank == 0) {
        matrix = generateMatrix(n, m);
    }

    std::vector<int> parallelRes = getColumnMinParallel(matrix, n, m);

    if (rank == 0) {
        std::vector<int> check(m);
        std::vector<int> t = transposeMatrix(matrix, n, m);

        for (int i = 0; i < m; i++) {
            check[i] = getMinInSequence(std::vector<int>(t.begin() + i * n,
            t.begin() + i * n + n));
        }

        ASSERT_EQ(check, parallelRes);
    }
}

TEST(matrix_operations, test2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;
    int n = 6;
    int m = 6;
    if (rank == 0) {
        matrix = generateMatrix(n, m);
    }

    std::vector<int> parallelRes = getColumnMinParallel(matrix, n, m);

    if (rank == 0) {
        std::vector<int> check(m);
        std::vector<int> t = transposeMatrix(matrix, n, m);

        for (int i = 0; i < m; i++) {
            check[i] = getMinInSequence(std::vector<int>(t.begin() + i * n,
            t.begin() + i * n + n));
        }

        ASSERT_EQ(check, parallelRes);
    }
}

TEST(matrix_operations, test3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;
    int n = 6;
    int m = 12;
    if (rank == 0) {
        matrix = generateMatrix(n, m);
    }

    std::vector<int> parallelRes = getColumnMinParallel(matrix, n, m);

    if (rank == 0) {
        std::vector<int> check(m);
        std::vector<int> t = transposeMatrix(matrix, n, m);

        for (int i = 0; i < m; i++) {
            check[i] = getMinInSequence(std::vector<int>(t.begin() + i * n,
            t.begin() + i * n + n));
        }

        ASSERT_EQ(check, parallelRes);
    }
}

TEST(matrix_operations, test4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;
    int n = 2;
    int m = 2;
    if (rank == 0) {
        matrix = generateMatrix(n, m);
    }

    std::vector<int> parallelRes = getColumnMinParallel(matrix, n, m);

    if (rank == 0) {
        std::vector<int> check(m);
        std::vector<int> t = transposeMatrix(matrix, n, m);

        for (int i = 0; i < m; i++) {
            check[i] = getMinInSequence(std::vector<int>(t.begin() + i * n,
            t.begin() + i * n + n));
        }

        ASSERT_EQ(check, parallelRes);
    }
}

int main(int argc, char** argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}
