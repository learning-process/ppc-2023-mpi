// Copyright 2023 Kosarev Egor
#include <mpi.h>
#include <gtest/gtest.h>
#include "task_1/kosarev_e_min_values_across_the_columns_matrix/min_values.h"

TEST(MPI_TESTS, Test_random1) {
    int rankProc = 0;
    int numProc = 0;
    int rows = 10;
    int cols = 15;
    std::vector<int>matr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    if (rankProc == 0) {
        matr = getRandomMatrix(rows, cols);
    }
    std::vector<int>parallelAns = GetMinColsParallel(matr, rows, cols);
    if (rankProc == 0) {
        std::vector<int> sequentialAns = GetMinCols(matr, rows, cols);
        ASSERT_EQ(sequentialAns, parallelAns);
    }
}

TEST(MPI_TESTS, Test_random2) {
    int rankProc = 0;
    int numProc = 0;
    int rows = 7;
    int cols = 7;
    std::vector<int>matr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    if (rankProc == 0) {
        matr = getRandomMatrix(rows, cols);
    }
    std::vector<int>parallelAns = GetMinColsParallel(matr, rows, cols);
    if (rankProc == 0) {
        std::vector<int> sequentialAns = GetMinCols(matr, rows, cols);
        ASSERT_EQ(sequentialAns, parallelAns);
    }
}

TEST(MPI_TESTS, Test_random3) {
    int rankProc = 0;
    int numProc = 0;
    int rows = 20;
    int cols = 9;
    std::vector<int>matr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    if (rankProc == 0) {
        matr = getRandomMatrix(rows, cols);
    }
    std::vector<int>parallelAns = GetMinColsParallel(matr, rows, cols);
    if (rankProc == 0) {
        std::vector<int> sequentialAns = GetMinCols(matr, rows, cols);
        ASSERT_EQ(sequentialAns, parallelAns);
    }
}

TEST(MPI_TESTS, Test_all_neg) {
    int rankProc = 0;
    int numProc = 0;
    int rows = 6;
    int cols = 6;
    std::vector<int>matr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    if (rankProc == 0) {
        matr = getRandomMatrix(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (matr[i * cols + j] > 0)
                    matr[i * cols + j] *= -1;
            }
        }
    }
    std::vector<int>parallelAns = GetMinColsParallel(matr, rows, cols);
    if (rankProc == 0) {
        std::vector<int> sequentialAns = GetMinCols(matr, rows, cols);
        ASSERT_EQ(sequentialAns, parallelAns);
    }
}

TEST(MPI_TESTS, Test_all_zero) {
    int rankProc = 0;
    int numProc = 0;
    int rows = 6;
    int cols = 6;
    std::vector<int>matr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    if (rankProc == 0) {
        matr.assign(rows * cols, 0);
    }
    std::vector<int>parallelAns = GetMinColsParallel(matr, rows, cols);
    if (rankProc == 0) {
        std::vector<int> sequentialAns = GetMinCols(matr, rows, cols);
        ASSERT_EQ(sequentialAns, parallelAns);
    }
}

int main(int argc, char** argv) {
    int result = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (rankProc != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
